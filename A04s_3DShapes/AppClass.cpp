#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("3D Shapes"); // Window Name
}

void AppClass::InitVariables(void)
{
	//Reserve Memory for a MyMeshClass object
	m_pPrimitive = new MyPrimitive();
	m_pPrimitive->GenerateCube(1.0f, REWHITE);
	m_pPrimitive->GenerateCone(1.0f, 1.0f, 12, REGREEN);
	m_pPrimitive->GenerateCylinder(1.0f, 2.0f, 7, REBLUE);
	m_pPrimitive->GenerateTube(1.0f, 0.7f, 2.0f, 7, REYELLOW);
	m_pPrimitive->GenerateSphere(1.0f, 5, RERED);
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

	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	
	m_pPrimitive->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), ToMatrix4(m_qArcBall));
	
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_pPrimitive); //Release the shape
	super::Release(); //release the memory of the inherited fields
}