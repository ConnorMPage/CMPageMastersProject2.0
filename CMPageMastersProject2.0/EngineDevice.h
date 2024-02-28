#pragma once
#include <d3d11.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <vector>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
class EngineDevice
{
public:
	EngineDevice(HWND hWnd);
	~EngineDevice() = default;


	void PresentFrame(bool vsync);



	Microsoft::WRL::ComPtr<ID3D11Device>pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>pContext;

	unsigned int GetBackbufferWidth() { return mBackbufferWidth; }
	unsigned int GetBackbufferHeight() { return mBackbufferHeight; }
private:
	int mBackbufferWidth;
	int mBackbufferHeight;


	
	Microsoft::WRL::ComPtr<IDXGISwapChain>pSwapChain;
	
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
};

