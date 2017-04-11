#include "AppClass.h"
//Needed for initializing Wiimotes
void on_state_change(wiimote &remote, state_change_flags changed, const wiimote_state &new_state)
{
	// we use this callback to set report types etc. to respond to key events
	//  (like the wiimote connecting or extensions (dis)connecting).

	// NOTE: don't access the public state from the 'remote' object here, as it will
	//		  be out-of-date (it's only updated via RefreshState() calls, and these
	//		  are reserved for the main application so it can be sure the values
	//		  stay consistent between calls).  Instead query 'new_state' only.

	// the wiimote just connected
	if (changed & CONNECTED)
	{
		// ask the wiimote to report everything (using the 'non-continous updates'
		//  default mode - updates will be frequent anyway due to the acceleration/IR
		//  values changing):

		// note1: you don't need to set a report type for Balance Boards - the
		//		   library does it automatically.

		// note2: for wiimotes, the report mode that includes the extension data
		//		   unfortunately only reports the 'BASIC' IR info (ie. no dot sizes),
		//		   so let's choose the best mode based on the extension status:
		if (new_state.ExtensionType != wiimote::BALANCE_BOARD)
		{
			if (new_state.bExtension)
				remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT); // no IR dots
			else
				remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);		//    IR dots
		}
	}
	// a MotionPlus was detected
	if (changed & MOTIONPLUS_DETECTED)
	{
		// enable it if there isn't a normal extension plugged into it
		// (MotionPlus devices don't report like normal extensions until
		//  enabled - and then, other extensions attached to it will no longer be
		//  reported (so disable the M+ when you want to access them again).
		if (remote.ExtensionType == wiimote_state::NONE) {
			bool res = remote.EnableMotionPlus();
			_ASSERT(res);
		}
	}
	// an extension is connected to the MotionPlus
	else if (changed & MOTIONPLUS_EXTENSION_CONNECTED)
	{
		// We can't read it if the MotionPlus is currently enabled, so disable it:
		if (remote.MotionPlusEnabled())
			remote.DisableMotionPlus();
	}
	// an extension disconnected from the MotionPlus
	else if (changed & MOTIONPLUS_EXTENSION_DISCONNECTED)
	{
		// enable the MotionPlus data again:
		if (remote.MotionPlusConnected())
			remote.EnableMotionPlus();
	}
	// another extension was just connected:
	else if (changed & EXTENSION_CONNECTED)
	{
#ifdef USE_BEEPS_AND_DELAYS
		Beep(1000, 200);
#endif
		// switch to a report mode that includes the extension data (we will
		//  loose the IR dot sizes)
		// note: there is no need to set report types for a Balance Board.
		if (!remote.IsBalanceBoard())
			remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT);
	}
	// extension was just disconnected:
	else if (changed & EXTENSION_DISCONNECTED)
	{
#ifdef USE_BEEPS_AND_DELAYS
		Beep(200, 300);
#endif
		// use a non-extension report mode (this gives us back the IR dot sizes)
		remote.SetReportType(wiimote::IN_BUTTONS_ACCEL_IR);
	}
}
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("SFML AND WIIYOURSELF CONTROLS (Esc to terminate)");
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);//What is up

	//Load a model into memory
	m_pMeshMngr->LoadModel("Lego\\Unikitty.bto", "Unikitty");

	//Connect Wiimotes
	for (int n = 0; n < m_nWiimotes; n++)
	{
		m_Wiimote[n].ChangedCallback = on_state_change;
		m_Wiimote[n].CallbackTriggerFlags =
			(state_change_flags)(CONNECTED | EXTENSION_CHANGED | MOTIONPLUS_CHANGED);
		if (!m_Wiimote[n].Connect(wiimote::FIRST_AVAILABLE))
		{
			Beep(500, 30); Sleep(1000);
			m_Wiimote[n].Connect(wiimote::FIRST_AVAILABLE);
		}
		if (m_Wiimote[n].IsConnected())
		{
			Beep(1000, 15);
			m_Wiimote[n].SetLEDs(n + 1);
		}
		m_Wiimote[n].IR.Mode = wiimote_state::ir::EXTENDED;
	}

	//Assume 360 controller... because it's a popular controller, updating the controller type in ProcessJoystick
	m_eController = Controller::C_360;
	m_uPad = 0;
	int nLimit = 0;
	sf::Joystick::update(); //We are not using a SFML window so we need to call this manually
							//If controller 0 is not connected try next to connect to the next controller, stop @ 3rd index
	while (!sf::Joystick::isConnected(m_uPad) && nLimit <= 3)
	{
		m_uPad++;
		nLimit++;
	}
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Update the information on the Mesh manager I will not check for collision detection so the argument is false
	m_pMeshMngr->Update();

	//Calculate the arcball
	ArcBall();
	
	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Modify model's ModelToWorld matrix
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), "Unikitty");

	//Add a model to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}