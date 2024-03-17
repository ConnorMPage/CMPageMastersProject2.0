#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <vector>
#include <DirectXMath.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
class EngineDevice
{
public:
	EngineDevice(HWND hWnd);
	~EngineDevice() = default;


	void PresentFrame(bool vsync);
	void beginScene(float r,float g, float b){
		const float colour[] = { r,g,b,1.0f };
		pContext->ClearRenderTargetView(pTarget.Get(), colour);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	HWND window;
	Microsoft::WRL::ComPtr<ID3D11Device>pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>pContext;

	unsigned int GetBackbufferWidth() { return mBackbufferWidth; }
	unsigned int GetBackbufferHeight() { return mBackbufferHeight; }

	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetWorldMatrix();
	DirectX::XMMATRIX GetOrthoMatrix();
private:
	int mBackbufferWidth;
	int mBackbufferHeight;


	
	Microsoft::WRL::ComPtr<IDXGISwapChain>pSwapChain;
	
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;


	DirectX::XMMATRIX mProjMatrix;
	DirectX::XMMATRIX mWorldMatrix;
	DirectX::XMMATRIX mOrthoMatrix;
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1f;
};

