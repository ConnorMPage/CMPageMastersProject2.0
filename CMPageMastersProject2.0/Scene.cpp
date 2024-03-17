#include "Scene.h"
#include "CommonGlobals.h"
#include "TerrainGen.h"
#include "EngineShaders.h"
#include "SceneCamera.h"

Scene::Scene()
{
	gTerrain = new TerrainGen();
	gTerrain->initialiseTerrain(DX->pDevice.Get());
	
	gEngineShader = new EngineShaders();
	gEngineShader->initialise(DX->pDevice.Get(), DX->window, L"vsBasicColour.cso", L"psBasicColour.cso");

	gCamera = new SceneCamera();


	DirectX::XMFLOAT3 cameraPos = {50.0f,2.0f,-7.0f};
	gCamera->SetPos(cameraPos);
}

Scene::~Scene()
{}

void Scene::RenderScene()
{
	DX->beginScene(0.0f, 0.0f, 0.5f);
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
	DX->PresentFrame(mLockFPS);
}
