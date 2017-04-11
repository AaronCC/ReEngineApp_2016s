#include "AppClass.h"
void AppClass::Update(void)
{
#pragma region DOES NOT NEED CHANGES
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma endregion
#pragma region FEEL FREE TO USE THIS CLOCK
	//Calculate delta and total times
	static double dTotalTime = 0.0; //Total time of the simulation
	double dDeltaTime = m_pSystem->LapClock(); //time difference between function calls
	dTotalTime += dDeltaTime; //Incrementing the time differences 
#pragma endregion
#pragma region YOUR CODE GOES HERE
	float fStartTime = 0.0f;
	float fEndTime = 5.0f;
	float fAngle = MapValue(static_cast<float>(dTotalTime), fStartTime, fEndTime, 0.0f, 360.0f);
	m_m4Steve = glm::rotate(IDENTITY_M4, fAngle, REAXISZ);

	static float fStartHeight = 0.0f;
	static float fEndHeight = 5.0f;
	float fPercent = MapValue(static_cast<float>(dTotalTime), fStartTime, fEndTime, 0.0f, 1.0f);
	float fHeight = glm::lerp(fStartHeight, fEndHeight, fPercent);
	m_m4Steve = m_m4Steve * glm::translate(vector3(0.0f, fHeight, 0.0f));
	if (fPercent > 1.0f)
	{
		std::swap(fStartHeight, fEndHeight);
		dTotalTime = 0.0;
	}
#pragma endregion
#pragma region DOES NOT NEED CHANGES
	//Set the model matrix
	m_pMeshMngr->SetModelMatrix(m_m4Steve, "Steve");
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("Seconds:");
	m_pMeshMngr->PrintLine(std::to_string(dTotalTime), RERED);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}
