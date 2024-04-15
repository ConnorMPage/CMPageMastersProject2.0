#include "EngineShaders.h"
#include"d3dcompiler.h"

EngineShaders::EngineShaders()
{
	
}

bool EngineShaders::initialise(ID3D11Device* gfxDevice, HWND window, const std::wstring& vsFilename, const std::wstring& psFilename)
{
	vsFile = vsFilename;
	psFile = psFilename;

	bool result = initialiseShader(gfxDevice, window, L"../Shaders\vsBasicColour.cso", L"psBasicColour.cso");
	return result;
}

void EngineShaders::InputLight(DirectX::XMFLOAT4 AmbientColour, DirectX::XMFLOAT4 DiffuseColour, DirectX::XMFLOAT3 Direction)
{
	mLightAmbientColour = AmbientColour;
	mLightDiffuseColour = DiffuseColour;
	mLightDirection = Direction;
}

bool EngineShaders::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	bool result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix);
	FinalShaderRender(deviceContext, indexCount);
	return result;
}

bool EngineShaders::initialiseShader(ID3D11Device* gfxDevice, HWND window, const std::wstring& vsFilename, const std::wstring& psFilename)
{
	HRESULT result;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	
	ID3D10Blob* errorMessage = 0;
	
	
	result =D3DReadFileToBlob(L"psBasicColour.cso", &pBlob);
	result =  gfxDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	
	result = D3DReadFileToBlob(L"vsBasicColour.cso", &pBlob);
	result = gfxDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	
	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "NORMAL";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	
	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);


	gfxDevice->CreateInputLayout(polygonLayout, numElements, pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), &pLayout);

	D3D11_BUFFER_DESC matrixBufferDesc;


	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	
	gfxDevice->CreateBuffer(&matrixBufferDesc, nullptr, &pMatrixBuffer);

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	gfxDevice->CreateSamplerState(&samplerDesc, &pSampleState);

	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	gfxDevice->CreateBuffer(&lightBufferDesc, nullptr, &pLightBuffer);

	return true;
}

bool EngineShaders::SetShaderParameters(ID3D11DeviceContext* deviceContext, DirectX::XMMATRIX worldMatrix, DirectX::XMMATRIX viewMatrix, DirectX::XMMATRIX projectionMatrix)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	worldMatrix = DirectX::XMMatrixTranspose(worldMatrix);
	viewMatrix = DirectX::XMMatrixTranspose(viewMatrix);
	projectionMatrix = DirectX::XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	result = deviceContext->Map(pMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	// Get a pointer to the data in the constant buffer. 
	dataPtr = (MatrixBufferType*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	dataPtr->world = worldMatrix;
	dataPtr->view = viewMatrix;
	dataPtr->projection = projectionMatrix;

	// Unlock the constant buffer.
	deviceContext->Unmap(pMatrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	deviceContext->VSSetConstantBuffers(bufferNumber, 1, &pMatrixBuffer);

	LightBufferType* dataPtr2;
	deviceContext->Map(pLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr2 = (LightBufferType*)mappedResource.pData;
	dataPtr2->AmbientColour = mLightAmbientColour;
	dataPtr2->DiffuseColour = mLightDiffuseColour;
	dataPtr2->Direction = mLightDirection;
	dataPtr2->padding = 0.0f;
	deviceContext->Unmap(pLightBuffer, 0);
	bufferNumber = 0;
	deviceContext->PSSetConstantBuffers(bufferNumber, 1, &pLightBuffer);

	return true;
	
}

void EngineShaders::FinalShaderRender(ID3D11DeviceContext* deviceContext, UINT indexCount)
{
	deviceContext->IASetInputLayout(pLayout.Get());

	
	deviceContext->VSSetShader(pVertexShader.Get(), NULL, 0);
	deviceContext->PSSetShader(pPixelShader.Get(), NULL, 0);

	
	deviceContext->DrawIndexed(indexCount,0u,0u);
}
