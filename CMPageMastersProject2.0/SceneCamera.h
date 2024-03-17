#pragma once
#include <DirectXMath.h>


class SceneCamera
{
public:
	SceneCamera();
	~SceneCamera() = default;

	void SetPos(DirectX::XMFLOAT3 newPos);
	void SetRotation(DirectX::XMFLOAT3 newRot);


	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetRotation();

	void RenderCam();
	DirectX::XMMATRIX GetViewMatrix();
private:
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMMATRIX mViewMatrix;
};

