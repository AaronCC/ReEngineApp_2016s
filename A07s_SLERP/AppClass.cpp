#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCameraMngr->SetPositionTargetAndView(vector3(12.12f, 28.52f, 11.34f), ZERO_V3, REAXISY);

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Loading the models
	m_pMeshMngr->LoadModel("Planets\\00_Sun.obj", "Sun");
	m_pMeshMngr->LoadModel("Planets\\03_Earth.obj", "Earth");
	m_pMeshMngr->LoadModel("Planets\\03A_Moon.obj", "Moon");

	//Setting the days duration
	m_fDay = 1.0f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

	//Starting with 3 orientations as 0 PI and 2 PI are basically the same
	//we require 2 stops
	quaternion qNoRotation = quaternion(vector3(0.0f, 0.0f, 0.0f));
	quaternion qHalfRotation = quaternion(vector3(0.0f, PI, 0.0f));
	quaternion qFullRotation = quaternion(vector3(0.0f, 2 * PI, 0.0f));

	//Earth Orbit
	static double fEarthOrbTime = 0.0f; //How much time has passed for the Earth?
	fEarthOrbTime += fCallTime; //keep the cumulative
	static int nEarthOrbits = 0; //count the number of orbits
	static bool nEarthOrbStep = false; //Route one
	double fEarthHalfOrbTime = 182.5f * m_fDay; //Earths orbit around the sun lasts 365 days / half the time for 2 stops
	float fEarthOrbPerc = MapValue(static_cast<float>(fEarthOrbTime), 0.0f, static_cast<float>(fEarthHalfOrbTime), 0.0f, 1.0f); //How much of the day we have (in a reason from 0 to 1)

	quaternion qEarthOrb; //Store the current orientation of the earth
	if (!nEarthOrbStep) //are we in the first stop?
		qEarthOrb = glm::mix(qNoRotation, qHalfRotation, fEarthOrbPerc);
	else//we are on the second
		qEarthOrb = glm::mix(qHalfRotation, qFullRotation, fEarthOrbPerc);

	//Once we are RE_DONE with the first stop set up the next and keep alternating
	if (fEarthOrbPerc >= 1.0f)
	{
		nEarthOrbStep = !nEarthOrbStep;
		fEarthOrbTime -= fEarthHalfOrbTime;
		if (!nEarthOrbStep)
			nEarthOrbits++;
	}

	//Earth Revolution (See coments for Earth Orbit, its the same procedure)
	static double fEarthRevTime = 0.0f;
	fEarthRevTime += fCallTime;
	static int nEarthRevolutions = 0;
	static bool nEarthRevStep = false;
	float fEarthHalfRevTime = 0.5f * m_fDay; // Move for Half a day
	float fEarthRevPerc = MapValue(static_cast<float>(fEarthRevTime), 0.0f, fEarthHalfRevTime, 0.0f, 1.0f);

	quaternion qEarthRev;
	if (!nEarthRevStep)
		qEarthRev = glm::mix(qNoRotation, qHalfRotation, fEarthRevPerc);
	else
		qEarthRev = glm::mix(qHalfRotation, qFullRotation, fEarthRevPerc);

	if (fEarthRevPerc >= 1.0f)
	{
		nEarthRevStep = !nEarthRevStep;
		fEarthRevTime -= fEarthHalfRevTime;
		if (!nEarthRevStep)
			nEarthRevolutions++;
	}

	//Moon Orbit (See coments for Earth Orbit, its the same procedure)
	static double fMoonOrbTime = 0.0f;
	fMoonOrbTime += fCallTime;
	static int nMoonOrbits = 0;
	static bool nMoonOrbStep = false;
	float fMoonHalfOrbTime = 14.0f * m_fDay; //Moon's orbit is 28 earth days, so half the time for half a route
	float fMoonOrbPerc = MapValue(static_cast<float>(fMoonOrbTime), 0.0f, fMoonHalfOrbTime, 0.0f, 1.0f);

	quaternion qMoonOrb;
	if (!nMoonOrbStep)
		qMoonOrb = glm::mix(qNoRotation, qHalfRotation, fMoonOrbPerc);
	else
		qMoonOrb = glm::mix(qHalfRotation, qFullRotation, fMoonOrbPerc);

	if (fMoonOrbPerc >= 1.0f)
	{
		nMoonOrbStep = !nMoonOrbStep;
		fMoonOrbTime -= fMoonHalfOrbTime;
		if (!nMoonOrbStep)
			nMoonOrbits++;
	}

	//Composing the matrices
	matrix4 m4Sun = glm::scale(vector3(5.936f));
	matrix4 m4Earth = glm::mat4_cast(qEarthOrb) * glm::translate(vector3(11.0f, 0.0f, 0.0f)) * glm::scale(vector3(0.524f));
	matrix4 m4Moon = m4Earth * glm::mat4_cast(qMoonOrb) * glm::translate(vector3(-2.0f, 0.0f, 0.0f)) * glm::scale(vector3(0.27f));
	m4Earth = m4Earth * glm::mat4_cast(qEarthRev);

	//Setting the matrices
	m_pMeshMngr->SetModelMatrix(m4Sun, "Sun");
	m_pMeshMngr->SetModelMatrix(m4Earth, "Earth");
	m_pMeshMngr->SetModelMatrix(m4Moon, "Moon");

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("\rTime:%.2f[s] Day:%.2f[s] E_Orbits:%d E_Revolutions:%d M_Orbits:%d         ", fRunTime, m_fDay, nEarthOrbits, nEarthRevolutions, nMoonOrbits);
	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	
	m_pMeshMngr->Print("Time:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime));

	m_pMeshMngr->Print("Day:");
	m_pMeshMngr->PrintLine(std::to_string(m_fDay));

	m_pMeshMngr->Print("E_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthOrbits));

	m_pMeshMngr->Print("E_Revolutions:");
	m_pMeshMngr->PrintLine(std::to_string(nEarthRevolutions));

	m_pMeshMngr->Print("M_Orbits:");
	m_pMeshMngr->PrintLine(std::to_string(nMoonOrbits));

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

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}