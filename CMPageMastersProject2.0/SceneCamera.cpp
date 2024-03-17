#include "SceneCamera.h"

SceneCamera::SceneCamera()
{
	mPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void SceneCamera::SetPos(DirectX::XMFLOAT3 newPos)
{
	mPosition = newPos;
}

void SceneCamera::SetRotation(DirectX::XMFLOAT3 newRot)
{
	mRotation = newRot;
}

DirectX::XMFLOAT3 SceneCamera::GetPos()
{
	return mPosition;
}

DirectX::XMFLOAT3 SceneCamera::GetRotation()
{
	return mRotation;
}

void SceneCamera::RenderCam()
{
	DirectX::XMFLOAT3 up;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	DirectX::XMFLOAT3 pos = mPosition;

	DirectX::XMFLOAT3 lookAt;
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;


	float pitch = mRotation.x * 0.0174532925f;
	float yaw = mRotation.y * 0.0174532925f;
	float roll = mRotation.z * 0.0174532925f;

	DirectX::XMMATRIX rotMatrix;
	rotMatrix = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
	DirectX::XMVECTOR lookAtv = { lookAt.x,lookAt.y,lookAt.z,0.0f };
	lookAtv = DirectX::XMVector3TransformCoord(lookAtv, rotMatrix);

	DirectX::XMVECTOR UPv = {up.x, up.y, up.z, 0.0f};
	UPv = DirectX::XMVector3TransformCoord(UPv, rotMatrix);

	DirectX::XMVECTOR PosV = { pos.x, pos.y, pos.z, 0.0f };

	lookAtv = DirectX::XMVectorAdd(PosV, lookAtv);

	mViewMatrix = DirectX::XMMatrixLookAtLH(PosV, lookAtv, UPv);
}

DirectX::XMMATRIX SceneCamera::GetViewMatrix()
{
	return mViewMatrix;
}
