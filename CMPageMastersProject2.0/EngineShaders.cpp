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

	polygonLayout[1].SemanticName = "COLOR";
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

	return true;
	
}

void EngineShaders::FinalShaderRender(ID3D11DeviceContext* deviceContext, UINT indexCount)
{
	deviceContext->IASetInputLayout(pLayout.Get());

	
	deviceContext->VSSetShader(pVertexShader.Get(), NULL, 0);
	deviceContext->PSSetShader(pPixelShader.Get(), NULL, 0);

	
	deviceContext->DrawIndexed(indexCount,0u,0u);
}
