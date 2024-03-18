#include "SceneCamera.h"

const float ROTATION_SPEED = 15.0f; 
const float MOVEMENT_SPEED = 50.0f; 


SceneCamera::SceneCamera()
{
	mPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	mRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void SceneCamera::CamControl(float frameTime, KeyCode turnUp, KeyCode turnDown, KeyCode turnLeft, KeyCode turnRight, KeyCode moveForward, KeyCode moveBackward, KeyCode moveLeft, KeyCode moveRight)
{
	if (KeyHeld(Key_D))
	{
		mPosition.x += MOVEMENT_SPEED * frameTime;
		
	}
	if (KeyHeld(Key_A))
	{
		mPosition.x -= MOVEMENT_SPEED * frameTime;
		
	}
	if (KeyHeld(Key_W))
	{
		
		
		mPosition.z += MOVEMENT_SPEED * frameTime;
	}
	if (KeyHeld(Key_S))
	{
		
		mPosition.z -= MOVEMENT_SPEED * frameTime;
	}

	if (KeyHeld(Key_Down))
	{
		mRotation.x += ROTATION_SPEED * frameTime; 
	}
	if (KeyHeld(Key_Up))
	{
		mRotation.x -= ROTATION_SPEED * frameTime;
	}
	if (KeyHeld(Key_Right))
	{
		mRotation.y += ROTATION_SPEED * frameTime;
	}
	if (KeyHeld(Key_Left))
	{
		mRotation.y -= ROTATION_SPEED * frameTime;
	}
	if (KeyHeld(Key_E))
	{
		mPosition.y += MOVEMENT_SPEED * frameTime;

	}
	if (KeyHeld(Key_Q))
	{
		mPosition.y -= MOVEMENT_SPEED * frameTime;

	}
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
