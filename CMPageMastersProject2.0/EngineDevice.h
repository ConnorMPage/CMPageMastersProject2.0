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
	EngineDevice(HWND hWnd); //Class Constructor 
	~EngineDevice() = default;//Class Destructor users default as device is stored a com ptr allow cleanup 


	void PresentFrame(bool vsync);// Function fo rpresenting the frame 


	void beginScene(float r,float g, float b){
		const float colour[] = { r,g,b,1.0f };//blue 

		//clears the buffers and sets colour to blue 
		pContext->ClearRenderTargetView(pTarget.Get(), colour);
		pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
	}

	HWND window;


	//public device and context for ease of access across project
	Microsoft::WRL::ComPtr<ID3D11Device>pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>pContext;

	//getter functions
	unsigned int GetBackbufferWidth() { return mBackbufferWidth; }
	unsigned int GetBackbufferHeight() { return mBackbufferHeight; }

	DirectX::XMMATRIX GetProjectionMatrix();
	DirectX::XMMATRIX GetWorldMatrix();
	DirectX::XMMATRIX GetOrthoMatrix();
private:
	//buffer dimensions 
	int mBackbufferWidth;
	int mBackbufferHeight;


	// com ptr for important pointers no need to release
	Microsoft::WRL::ComPtr<IDXGISwapChain>pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	//world matrices 
	DirectX::XMMATRIX mProjMatrix;
	DirectX::XMMATRIX mWorldMatrix;
	DirectX::XMMATRIX mOrthoMatrix;

	//near and far clip 
	const float SCREEN_DEPTH = 1000.0f;
	const float SCREEN_NEAR = 0.1f;
};

