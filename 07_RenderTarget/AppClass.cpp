#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Render Tagets (Render to texture)"); // Window Name
	m_v4ClearColor = vector4(RECORNFLOWERBLUE, 0.0f);
	//m_pSystem->SetWindowFullscreen(RESOLUTIONS::C_1920x1080_16x9_FULLHD);
}

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
	
	m_bUsingTexture = false;
	m_pTextureMngr = TextureManagerSingleton::GetInstance();
	if (m_bUsingTexture)
	{
		m_sTexture = "Starry Night.jpg";
		m_pTextureMngr->LoadTexture(m_sTexture);
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

	double dDeltaTime = m_pSystem->LapClock();
	static double dTotalTime = 0;
	dTotalTime += dDeltaTime;

	static uint nStart = 0;
	static uint nEnd = 1;
	std::vector<vector3> lLocations;
	lLocations.push_back(vector3(-5.0f,-3.0f,-3.0f));
	lLocations.push_back(vector3(-5.0f, 5.0f, 3.0f));
	lLocations.push_back(vector3( 5.0f, 5.0f, 3.0f));
	lLocations.push_back(vector3( 5.0f,-3.0f, 3.0f));
	lLocations.push_back(vector3( 0.0f, 7.0f, 0.0f));
	float fPercentage = MapValue(dTotalTime, 0.0, 3.0, 0.0, 1.0);
	vector3 v3Position = glm::lerp(lLocations[nStart], lLocations[nEnd], fPercentage);
	m_pMeshMngr->AddSphereToRenderList(glm::translate(v3Position), RERED, WIRE);
	if (fPercentage >= 1.0f)
	{
		nStart++;
		nEnd++;
		if (nStart > lLocations.size() - 1)
			nStart = 0;
		if (nEnd > lLocations.size() - 1)
			nEnd = 0;
		dTotalTime = 0.0;
	}

	//Set the model matrix for the first model to be the arcball
	m_pMeshMngr->SetModelMatrix(ToMatrix4(m_qArcBall), 0);
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//Set the render target
	m_pMeshMngr->SetRenderTarget(m_nFrameBuffer, m_nDepthBuffer, m_nDawingTexture); //(To the first render target)
	
	//clear the screen (or render target)
	ClearScreen();

	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());

	//Render the render list
	m_pMeshMngr->Render(m_fDepth);

	//Set the render target
	m_pMeshMngr->SetRenderTarget();//(to the screen)
	
	//clear the screen (or render target)
	ClearScreen();

	if (m_bUsingTexture)
	{
		int nTexture = m_pTextureMngr->IdentifyTexure(m_sTexture);
		GLuint temp = m_pTextureMngr->ReturnGLIndex(nTexture);
		m_pMeshMngr->RenderTexture(temp);
	}
	else
		m_pMeshMngr->RenderTexture(m_nDawingTexture);
	
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}