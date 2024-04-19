#pragma once
#include "DirectXMath.h"
class Light
{
public:
	struct LightData {//hold all light values in a struct
		DirectX::XMFLOAT4 AmbientColour;
		DirectX::XMFLOAT4 DiffuseColour;
		DirectX::XMFLOAT3 Direction;
	};

	Light();//constructor

	~Light() = default;//destructor 

	//setter functions 
	void SetAmbient(DirectX::XMFLOAT4 colour);
	void SetDiffuse(DirectX::XMFLOAT4 colour);
	void SetDirection(DirectX::XMFLOAT3 dir);

	//getter functiom
	LightData GetLightData();
private:

	//struct object
	LightData mLightData;
};

