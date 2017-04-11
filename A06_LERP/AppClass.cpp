#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);
	
	// Add waypoint vectors
	m_points.push_back(vector3(-4.0f, -2.0f, 5.0f));
	m_points.push_back(vector3(1.0f, -2.0f, 5.0f));
	m_points.push_back(vector3(-3.0f, -1.0f, 3.0f));
	m_points.push_back(vector3(2.0f, -1.0f, 3.0f));
	m_points.push_back(vector3(-2.0f, 0.0f, 0.0f));
	m_points.push_back(vector3(3.0f, 0.0f, 0.0f));
	m_points.push_back(vector3(-1.0f, 1.0f, -3.0f));
	m_points.push_back(vector3(4.0f, 1.0f, -3.0f));
	m_points.push_back(vector3(0.0f, 2.0f, -5.0f));
	m_points.push_back(vector3(5.0f, 2.0f, -5.0f));
	m_points.push_back(vector3(1.0f, 3.0f, -5.0f));

	for (int i = 0; i < m_points.size(); i++) // Initialize mat4s and primitives
	{
		m_sMats.push_back(IDENTITY_M4);
		m_sMats[i] = glm::translate(IDENTITY_M4, m_points[i]);
		m_spheres.push_back(new PrimitiveClass());
		m_spheres[i]->GenerateSphere(0.1f, 5, RERED);
	}

	m_pIndex = 0;

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	//m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	//Use the Clock
	static DWORD startTimeSystem = GetTickCount();
	DWORD startTimeApplication = GetTickCount() - startTimeSystem;
	float timer = startTimeApplication / 1000.0f;
	m_pMeshMngr->PrintLine("Time is: " + std::to_string(timer));

	//Lerp

	matrix4 m4WallEye;
	float timerMapped = MapValue(timer, 0.f, 1.f, 0.f, 1.f);
	if (timerMapped >= 1.0f) // Reached the next waypoint
	{
		m_pIndex++; // Incriment the index
		m_pIndex = m_pIndex == m_points.size() ? 0 : m_pIndex; // Reset the index
		startTimeSystem += startTimeApplication; // New start time
		timerMapped = 0.0f; // The start of the next waypoint
	}
	if(m_pIndex < m_points.size() - 1) // Not at the end
	{
		vector3 v3lerp = glm::lerp(m_points[m_pIndex], m_points[m_pIndex + 1], timerMapped);
		m4WallEye = glm::translate(v3lerp);
	}
	else // Restart
	{
		vector3 v3lerp = glm::lerp(m_points[m_pIndex], m_points[0], timerMapped);
		m4WallEye = glm::translate(v3lerp);
	}
	m_pMeshMngr->SetModelMatrix(m4WallEye, "WallEye");
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	for (int i = 0; i < m_points.size(); i++) // Render the waypoints
	{
		m_spheres[i]->Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_sMats[i]);
	}
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	delete &m_sMats;
	delete &m_points;
	delete &m_spheres;
	super::Release(); //release the memory of the inherited fields
}