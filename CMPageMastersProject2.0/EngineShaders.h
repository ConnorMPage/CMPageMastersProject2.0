#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <string>
class EngineShaders
{
public:
	EngineShaders();//constructor 
	~EngineShaders() = default;//destructor 

	//function that runs the private initalise
	bool Initialise(ID3D11Device* gfxDevice, HWND window, const std::wstring& vsFilename, const std::wstring& psFilename);

	//inputs light data intop the lighting variables 
	void InputLight(DirectX::XMFLOAT4 AmbientColour, DirectX::XMFLOAT4 DiffuseColour, DirectX::XMFLOAT3 Direction);

	//sets per frame constants and draws indexed
	bool RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX worldMatrix,
		DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix, int scale);

	
private:
	struct MatrixBufferType//cbuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
		float scaler = 1;
	};
	struct LightBufferType {//struct for light c buffer
		DirectX::XMFLOAT4 AmbientColour;
		DirectX::XMFLOAT4 DiffuseColour;
		DirectX::XMFLOAT3 Direction;
		float padding;
	};


	struct UpdateBufferType//ignore doesnt do anything 
	{
		float Scaler;
	};

	//initialise the shader values and create buffers 
	bool InitialiseShader(ID3D11Device* gfxDevice, HWND window, const std::wstring& vsFilename, const std::wstring& psFilename);

	//per frame constants are set here
	bool SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix,
		DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix,int scale);

	//sets the buffers and draws indexed
	void FinalShaderRender(ID3D11DeviceContext* deviceContext, UINT indexCount);


	 std::wstring vsFile;
	 std::wstring psFile;
	 //com ptr used to reduce cleanup 
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pLayout;
	
	//buffer pointers 
	ID3D11SamplerState* pSampleState;
	ID3D11Buffer* pMatrixBuffer;
	ID3D11Buffer* pLightBuffer;
	ID3D11Buffer* pUpdateBuffer;

	//values of lighting from light class
	DirectX::XMFLOAT4 mLightAmbientColour;
	DirectX::XMFLOAT4 mLightDiffuseColour;
	DirectX::XMFLOAT3 mLightDirection;

	
	MatrixBufferType* dataPtr;
};

