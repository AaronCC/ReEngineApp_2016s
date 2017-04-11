#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Bounding Spheres DEMO");
}

void AppClass::InitVariables(void)
{
	//Set the camera position
	m_pCameraMngr->SetPositionTargetAndView(
		vector3(0.0f, 2.5f, 15.0f),//Camera position
		vector3(0.0f, 2.5f, 0.0f),//What Im looking at
		REAXISY);//What is up
	//Load a model onto the Mesh manager
	m_pMeshMngr->LoadModel("Minecraft\\Zombie.obj", "Zombie");
	m_pBS0 = new MyBoundingSphereClass(m_pMeshMngr->GetVertexList("Zombie"));
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
	matrix4 m4Sphere = glm::translate(m_pBS0->m_v3Center) * 
		glm::scale(vector3(m_pBS0->m_fRadius) * 2.0f);
	m_pMeshMngr->AddSphereToRenderList(m4Sphere, RERED, WIRE);
	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(glm::translate(vector3(1, 0, 0)), "Zombie");
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddSkyboxToRenderList();
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Radius: ");
	m_pMeshMngr->PrintLine(std::to_string(m_pBS0->m_fRadius), RERED);
	m_pMeshMngr->Print("Center: (");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.x), RERED);
	m_pMeshMngr->Print(" , ");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.y), RERED);
	m_pMeshMngr->Print(" , ");
	m_pMeshMngr->Print(std::to_string(m_pBS0->m_v3Center.z), RERED);
	m_pMeshMngr->PrintLine(")");

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
	SafeDelete(m_pBS0);

	if (m_pBS0 != nullptr)
	{
		delete m_pBS0;
		m_pBS0 = nullptr;
	}
	super::Release(); //release the memory of the inherited fields
}