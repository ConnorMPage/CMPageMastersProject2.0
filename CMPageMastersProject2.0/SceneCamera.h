#pragma once
#include <DirectXMath.h>
#include "UserInputDevice.h"

class SceneCamera
{
public:
	SceneCamera();//constructor
	~SceneCamera() = default;//destructor 

	void CamControl(float frameTime, KeyCode turnUp, KeyCode turnDown, KeyCode turnLeft, KeyCode turnRight,
		KeyCode moveForward, KeyCode moveBackward, KeyCode moveLeft, KeyCode moveRight);//input events 

	void SetPos(DirectX::XMFLOAT3 newPos);//set position 
	void SetRotation(DirectX::XMFLOAT3 newRot);//set rotation


	//getter functions 
	DirectX::XMFLOAT3 GetPos();
	DirectX::XMFLOAT3 GetRotation();
	DirectX::XMMATRIX GetViewMatrix();

	void RenderCam();//renders from current position

	
private:
	//positions and matrix
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mRotation;
	DirectX::XMMATRIX mViewMatrix;
};

