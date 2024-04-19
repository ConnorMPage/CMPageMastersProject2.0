#include "Scene.h"
#include "CommonGlobals.h"
#include "TerrainGen.h"
#include "EngineShaders.h"
#include "SceneCamera.h"
#include "UserInputDevice.h"
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include "HeightmapGenerator.h"
#include "Light.h"
#include <shobjidl.h> 
Scene::Scene()
{ // initialise scene objects 
	gHGen = new HeightmapGenerator();
	

	gTerrain = new TerrainGen();
	gEngineShader = new EngineShaders();
	gCamera = new SceneCamera();


	DirectX::XMFLOAT3 cameraPos = {50.0f,2.0f,-7.0f};
	gCamera->SetPos(cameraPos);


	gLight = new Light();
	gLight->SetAmbient(DirectX::XMFLOAT4(0.05f, 0.05f, 0.05f, 1.0f));
	gLight->SetDiffuse(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	gLight->SetDirection(DirectX::XMFLOAT3(0.0f, 0.0f, 0.75f));

	//InitialiseTerrainGenerator();
}

Scene::~Scene()
{}

void Scene::RenderScene()
{//render each frame 
	DX->beginScene(0.0f, 0.0f, 0.5f);

	ImGui_ImplDX11_NewFrame(); 
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(DX->GetBackbufferWidth());
	vp.Height = static_cast<FLOAT>(DX->GetBackbufferHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DX->pContext->RSSetViewports(1, &vp);

	gCamera->RenderCam();
	if (mDisplayTerrain)
	{
		gEngineShader->InputLight(gLight->GetLightData().AmbientColour, gLight->GetLightData().DiffuseColour, gLight->GetLightData().Direction);
		gTerrain->RenderTerrain(DX->pContext.Get());
		int index = gTerrain->getIndexCount();
		if (!gEngineShader->RenderShader(DX->pContext.Get(), gTerrain->getIndexCount(), DX->GetWorldMatrix(), gCamera->GetViewMatrix(), DX->GetProjectionMatrix(),mRuntimeScale)) return;
	}


	// IMGUI
	ImGui::Begin("Simulation Controls", 0, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderInt("HeightMap Width", &mHeightMapWidth, MINMAPSIZE, MAXMAPSIZE);
	ImGui::SliderInt("HeightMap Height", &mHeightMapHeight, MINMAPSIZE, MAXMAPSIZE);
	ImGui::SliderInt("HeightMap Scale", &mHeightMapScale, MINMAPSCALE, MAXMAPSCALE);
	ImGui::SliderInt("Octaves", &mOctavesAmount, MINOCTAVES, MAXOCTAVES);
	if (ImGui::Button("Generate HeightMap", ImVec2(350, 50))) CreateHeightmap();
	if (ImGui::Button("Select File", ImVec2(350, 50)))OpenFile();
	ImGui::Checkbox("Normalise Heightmap", &mNormalised);
	if (ImGui::Button("Initialise Terrain", ImVec2(350, 50)))if (!mTerrainSet) mTerrainSet = InitialiseTerrainGenerator();
	
	if (ImGui::Button("Reset", ImVec2(350, 50))) Reset();
	ImGui::SliderInt("Terrain Scale", &mRuntimeScale, MINMAPSCALE, MAXMAPSCALE);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	DX->PresentFrame(mLockFPS);
}

void Scene::Update(float frameTime)
{//update scene objects 
	gCamera->CamControl(frameTime, Key_Up, Key_Down, Key_Left, Key_Right, Key_W, Key_S, Key_A, Key_D);
	
}

bool Scene::InitialiseTerrainGenerator()
{
	bool result;
	if (mHMapAddress == "")
	{
		const auto mErrorMessage = "Error! you have not Loaded a HeightMap";
		MessageBoxA(DX->window, mErrorMessage, NULL, MB_OK);
		return false;
	}
	
	result = gTerrain->InitialiseTerrain(DX->pDevice.Get(), mHMapAddress,mNormalised);
	result = gEngineShader->Initialise(DX->pDevice.Get(), DX->window, L"vsBasicColour.cso", L"psBasicColour.cso");
	mDisplayTerrain = true;
	return result;
}

bool Scene::CreateHeightmap()
{
	bool result;
	result = gHGen->GenerateHeightmap(mHeightMapHeight, mHeightMapWidth, mHeightMapScale,mOctavesAmount);
	mHMapAddress = "../CMPageMastersProject2.0/Data/GeneratedHeightMap.bmp";
	return result;
}

bool Scene::OpenFile()
{
	// crreate file instance
	HRESULT f_SysHr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	if (FAILED(f_SysHr))
		return FALSE;

	// create dialoge box
	IFileOpenDialog* f_FileSystem;
	f_SysHr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&f_FileSystem));
	if (FAILED(f_SysHr)) {
		CoUninitialize();
		return FALSE;
	}

	//  show box
	f_SysHr = f_FileSystem->Show(NULL);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  retrieve selected file name 
	IShellItem* f_Files;
	f_SysHr = f_FileSystem->GetResult(&f_Files);
	if (FAILED(f_SysHr)) {
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}

	//  store file name
	PWSTR f_Path;
	f_SysHr = f_Files->GetDisplayName(SIGDN_FILESYSPATH, &f_Path);
	if (FAILED(f_SysHr)) {
		f_Files->Release();
		f_FileSystem->Release();
		CoUninitialize();
		return FALSE;
	}
	std::wstring path(f_Path);
	std::string c(path.begin(), path.end());
	mHMapAddress = c;

	const size_t slash = mHMapAddress.find_last_of("/\\");
	mHMapName = mHMapAddress.substr(slash + 1);


	CoTaskMemFree(f_Path);
	f_Files->Release();
	f_FileSystem->Release();
	CoUninitialize();
	return TRUE;
	return false;
}

void Scene::Reset()
{//delete and reset 
	delete gTerrain;
	gTerrain = new TerrainGen();
	mDisplayTerrain = false;
	mTerrainSet = false;
}
