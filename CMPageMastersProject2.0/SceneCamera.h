#pragma once
#include <DirectXMath.h>
#include "UserInputDevice.h"

class SceneCamera
{
public:
	SceneCamera();
	~SceneCamera() = default;

	void CamControl(float frameTime, KeyCode turnUp, KeyCode turnDown, KeyCode turnLeft, KeyCode turnRight,
		KeyCode moveForward, KeyCode moveBackward, KeyCode moveLeft, KeyCode moveRight);

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

