#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("Instance Rendering - Example");
}

void AppClass::InitVariables(void)
{
	//Reserve Memory for a MyMeshClass object
	m_pMesh = new MyMesh();

	m_pMesh->AddVertexPosition(vector3(0.0f, 0.0f, 0.0f));
	m_pMesh->AddVertexPosition(vector3(5.0f, 0.0f, 0.0f));
	m_pMesh->AddVertexPosition(vector3(0.0f, 5.0f, 0.0f));

	m_pMesh->AddVertexColor(REGREEN);
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexColor(REBLUE);

	m_pMesh->CompileOpenGL3X();

	m_fMatrixArray = new float[m_nObjects * 16];
	for (int nObject = 0; nObject < m_nObjects; nObject++)
	{
		const float* m4MVP = glm::value_ptr(
			glm::translate(vector3(0.01f * -nObject, 0.0f, 1.0f * -nObject)) *
			glm::rotate(IDENTITY_M4, nObject * 5.0f, REAXISZ)
			);
		memcpy(&m_fMatrixArray[nObject * 16], m4MVP, 16 * sizeof(float));
	}

}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Call the Arcball method
	ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
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

	m_pMesh->RenderList(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_fMatrixArray, m_nObjects);//Rendering nObjects

	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}