#include "EngineDevice.h"
#include <DirectXMath.h>
#include <stdexcept>

namespace dxm = DirectX;
EngineDevice::EngineDevice(HWND hWnd)
{
	HRESULT hRes = S_OK;

	// Get the window size
	RECT wind;
	GetClientRect(hWnd, &wind);
	mBackbufferWidth = wind.right - wind.left;
	mBackbufferHeight = wind.bottom - wind.top;


	//setup swapchain
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferDesc.Width = mBackbufferWidth;
	scDesc.BufferDesc.Height = mBackbufferHeight;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 0;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 1;
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = TRUE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = 0;

	window = hWnd;
	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	//create the device and swapchain
	hRes = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scDesc,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pContext
	);

	if (FAILED(hRes))
	{
		throw std::runtime_error("Creation of Device and Swapchain failed");
	}
	Microsoft::WRL::ComPtr<ID3D11Resource>mBackBufferTexture;

	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &mBackBufferTexture);

	hRes = pDevice->CreateRenderTargetView(mBackBufferTexture.Get(), nullptr, &pTarget);

	if (FAILED(hRes))
	{
		throw std::runtime_error("Render target View Creation failed");
	}


	//mBackBufferTexture->Release();

	// create depth stensil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stensil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = mBackbufferWidth;
	descDepth.Height = mBackbufferHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDSV);

	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), pDSV.Get());

	float fov = DirectX::XM_PI / 4.0f;
	float screenAspect = mBackbufferWidth / mBackbufferHeight;

	mWorldMatrix = DirectX::XMMatrixIdentity();
	mOrthoMatrix = DirectX::XMMatrixOrthographicLH(mBackbufferWidth, mBackbufferHeight, SCREEN_NEAR, SCREEN_DEPTH);
	mProjMatrix = DirectX::XMMatrixPerspectiveFovLH(fov, screenAspect, SCREEN_NEAR, SCREEN_DEPTH);
}

void EngineDevice::PresentFrame(bool vsync)
{
	
	pSwapChain->Present(vsync ? 1 : 0, vsync ? 0 : DXGI_PRESENT_DO_NOT_WAIT);

}

DirectX::XMMATRIX EngineDevice::GetProjectionMatrix()
{
	return mProjMatrix;
}

DirectX::XMMATRIX EngineDevice::GetWorldMatrix()
{
	return mWorldMatrix;
}

DirectX::XMMATRIX EngineDevice::GetOrthoMatrix()
{
	return mOrthoMatrix;
}
