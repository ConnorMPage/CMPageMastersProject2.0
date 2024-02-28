#include "Scene.h"
#include "CommonGlobals.h"

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::RenderScene()
{
	D3D11_VIEWPORT vp;
	vp.Width = static_cast<FLOAT>(DX->GetBackbufferWidth());
	vp.Height = static_cast<FLOAT>(DX->GetBackbufferHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	DX->pContext->RSSetViewports(1, &vp);


	DX->PresentFrame(mLockFPS);
}
