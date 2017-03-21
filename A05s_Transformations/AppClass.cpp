#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("3D Transformations");
	m_v4ClearColor = vector4(0.0f, 0.0f, 0.0f, 0.0f);
}

void AppClass::InitVariables(void)
{
	m_m4Sun = IDENTITY_M4;
	m_m4Earth = IDENTITY_M4;
	m_m4Moon = IDENTITY_M4;

	m_pSun = new PrimitiveClass();
	m_pEarth = new PrimitiveClass();
	m_pMoon = new PrimitiveClass();

	m_pSun->GenerateSphere(5.936f, 5, REYELLOW);
	m_pEarth->GenerateTube(0.524f, 0.45f, 0.3f, 10, REBLUE);
	m_pMoon->GenerateTube(0.524f * 0.27f, 0.45f * 0.27f, 0.3f * 0.27f, 10, REWHITE);
}

void AppClass::Update(void)
{
	//Sets the camera
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 25.0f, 0.0f), vector3(0.0f, 0.0f, 0.0f), -REAXISZ);

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

	//This matrices will just orient the objects to the camera
	matrix4 rotateX = glm::rotate(IDENTITY_M4, 90.0f, vector3(1.0f, 0.0f, 0.0f));
	matrix4 rotateY = glm::rotate(IDENTITY_M4, 90.0f, vector3(0.0f, 1.0f, 0.0f));

	//This matrices will hold the relative transformation of the Moon and the Earth
	matrix4 distanceEarth = glm::translate(11.0f, 0.0f, 0.0f);
	matrix4 distanceMoon = glm::translate(2.0f, 0.0f, 0.0f);

	//Earth's orbit around the Sun
	matrix4 orbitEarth = glm::rotate(IDENTITY_M4, m_fEarthTimer, vector3(0.0f, 1.0f, 0.0f));
	//Earth's rotation on its own axis
	matrix4 rotationEarth = glm::rotate(IDENTITY_M4, m_fEarthTimer * 28.0f, vector3(0.0f, 1.0f, 0.0f));

	//Moon's orbit around the Earth
	matrix4 orbitMoon = glm::rotate(IDENTITY_M4, m_fMoonTimer, vector3(0.0f, -1.0f, 0.0f));
	//Moon's rotation around its own axis
	matrix4 rotationMoon = glm::rotate(IDENTITY_M4, m_fMoonTimer, vector3(0.0f, 1.0f, 0.0f));

	//I will calculate the Earth position in space relative to the Sun (which is in global space)
	matrix4 earthsSpace = orbitEarth * distanceEarth;//the translation depends on the orientation of the space (Earths orbit)
	m_m4Earth = earthsSpace * rotationEarth;//the rotation of the Earth will depend on the new space
	m_m4Earth = m_m4Earth * rotateX;//Will orient the Earth to the camera now

	//I will calculate the moon's position in space relative to the Earth (earthsSpace)
	matrix4 moonsSpace = orbitMoon * distanceMoon;//the moon's translation depends on its orientation (Moons orbit)
	m_m4Moon = earthsSpace * moonsSpace;//Then we place the moons space in therms of the earth space
	m_m4Moon = m_m4Moon * rotateY * rotateX;//Then we orient the Moon to the camera)

	printf("Earth Day: %.3f, Moon Day: %.3f\r", m_fEarthTimer, m_fMoonTimer);//print the Frames per Second

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("Earth Day: ", REWHITE);
	m_pMeshMngr->PrintLine(std::to_string(m_fEarthTimer), REBLUE);
	m_pMeshMngr->Print("Moon Day: ", REWHITE);
	m_pMeshMngr->PrintLine(std::to_string(m_fMoonTimer), REBLUE);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);

	m_fMoonTimer++;//Increase moon timer
	m_fEarthTimer = m_fMoonTimer / 28.0f; //divide by the moon's day
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Renders the meshes using the specified position given by the matrix and in the specified color
	m_pSun->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_m4Sun);
	m_pEarth->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_m4Earth);
	m_pMoon->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_m4Moon);

	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_pEarth);
	SafeDelete(m_pMoon);
	SafeDelete(m_pSun);

	super::Release(); //release the memory of the inherited fields
}