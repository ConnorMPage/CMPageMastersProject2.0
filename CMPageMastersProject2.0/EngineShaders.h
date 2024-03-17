#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
class EngineShaders
{
public:
	EngineShaders();
	~EngineShaders() = default;

	bool initialise(ID3D11Device* gfxDevice, HWND window, const std::wstring& vsFilename, const std::wstring& psFilename);

	bool RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX worldMatrix,
		DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
private:
	struct MatrixBufferType
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

	bool initialiseShader(ID3D11Device* gfxDevice, HWND window, const std::wstring& vsFilename, const std::wstring& psFilename);
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix,
		DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix);
	void FinalShaderRender(ID3D11DeviceContext* deviceContext, UINT indexCount);


	 std::wstring vsFile;
	 std::wstring psFile;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;
	ID3D11Buffer* pMatrixBuffer;
	
};

