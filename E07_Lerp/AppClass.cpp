#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Sandbox"); // Window Name

	// Set the clear color based on Microsoft's CornflowerBlue (default in XNA)
	//if this line is in Init Application it will depend on the .cfg file, if it
	//is on the InitVariables it will always force it regardless of the .cfg
	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
	m_pSystem->SetWindowResolution(RESOLUTIONS::C_1280x720_16x9_HD);
	//m_pSystem->SetWindowFullscreen(); //Sets the window to be fullscreen
	//m_pSystem->SetWindowBorderless(true); //Sets the window to not have borders
}
//template <class T>
//static T MapValue(T valueToMap, T originalScale_min, T originalScale_max, T mappedScale_min, T mappedScale_max)
//{
//	return (valueToMap - originalScale_min) * (mappedScale_max - mappedScale_min) / (originalScale_max - originalScale_min) + mappedScale_min;
//}
void AppClass::InitVariables(void)
{
	//Reset the selection to -1, -1
	m_selection = std::pair<int, int>(-1, -1);
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	m_pMeshMngr->LoadModel("Lego\\Unikitty.bto", "Unikitty");
	m_pCameraMngr->SetPosition(vector3(0.0f, 0.0f, 35.0f));
	srand(time(NULL));
	m_nObjects = rand() % 23 + 5;
	vector3 v3Start = vector3(-m_nObjects, 0.0f, 0.0f);
	vector3 v3End = vector3(m_nObjects, 0.0f, 0.0f);

	m_pSphere = new PrimitiveClass[m_nObjects];
	m_pMatrix = new matrix4[m_nObjects];
	for (int i = 0; i < m_nObjects; i++)
	{
		float fPercent = MapValue(static_cast<float>(i), 0.0f, static_cast<float>(m_nObjects), 0.0f, 1.0f);
		m_pSphere[i] = PrimitiveClass();
		m_pSphere->GenerateSphere(1.f, 5, vector3(fPercent, 0.0f, 0.0f));
		m_pMatrix[i] = glm::translate(IDENTITY_M4, glm::lerp(v3Start, v3End, fPercent));
	}
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

	//Call the arcball method
	ArcBall();

	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), 0);

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);

	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	//m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	for (int i = 0; i < m_nObjects; i++)
	{
		m_pSphere[i].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_pMatrix[i]);
	}
	
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}