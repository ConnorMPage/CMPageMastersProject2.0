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

Scene::Scene()
{ // initialise scene objects 
	/*gHGen = new HeightmapGenerator();
	gHGen->GenerateHeightmap(256, 256);*/

	gTerrain = new TerrainGen();
	gTerrain->initialiseTerrain(DX->pDevice.Get());
	
	gEngineShader = new EngineShaders();
	gEngineShader->initialise(DX->pDevice.Get(), DX->window, L"vsBasicColour.cso", L"psBasicColour.cso");

	gCamera = new SceneCamera();


	DirectX::XMFLOAT3 cameraPos = {50.0f,2.0f,-7.0f};
	gCamera->SetPos(cameraPos);


	gLight = new Light();
}

Scene::~Scene()
{}

void Scene::RenderScene()
{
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

	gTerrain->RenderTerrain(DX->pContext.Get());
	int index = gTerrain->getIndexCount();
	if (!gEngineShader->RenderShader(DX->pContext.Get(), gTerrain->getIndexCount(), DX->GetWorldMatrix(), gCamera->GetViewMatrix(), DX->GetProjectionMatrix())) return;
	
	
	int MaxParticles = 10;


	// IMGUI
	ImGui::Begin("Simulation Controls", 0, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::SliderInt("Elevation Mulitplier", &numParticles, 1, MaxParticles);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	DX->PresentFrame(mLockFPS);
}

void Scene::Update(float frameTime)
{//update scene objects 
	gCamera->CamControl(frameTime, Key_Up, Key_Down, Key_Left, Key_Right, Key_W, Key_S, Key_A, Key_D);
}
