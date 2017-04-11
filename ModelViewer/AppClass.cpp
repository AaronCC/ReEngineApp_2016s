#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Model Viewer"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPosition(REAXISZ * 15.0f);

	m_sSelectedObject = "";

	m_selection = std::pair<int, int>(-1, -1);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC)
		CameraRotation();

	ArcBall();

	if (m_sSelectedObject != "")
	{
		m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qOrientation * m_qArcBall), m_sSelectedObject);
	}

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList(m_sSelectedObject);

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName());
	m_pMeshMngr->Print("Model: ");
	m_pMeshMngr->PrintLine(m_sSelectedObject, REGREEN);
	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	m_pMeshMngr->Print("State: ");
	if (m_sSelectedObject != "")
	{
		InstanceClass* pInstance = m_pMeshMngr->GetInstanceByName(m_sSelectedObject);
		int nState = pInstance->GetCurrentState();
		m_pMeshMngr->PrintLine(std::to_string(nState), REGREEN);
	}
	else
	{
		m_pMeshMngr->PrintLine("", REGREEN);
	}
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
