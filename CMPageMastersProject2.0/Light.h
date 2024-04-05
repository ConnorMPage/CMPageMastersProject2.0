#pragma once
#include "DirectXMath.h"
class Light
{
public:
	struct LightData {
		DirectX::XMFLOAT4 AmbientColour;
		DirectX::XMFLOAT4 DiffuseColour;
		DirectX::XMFLOAT3 Direction;
	};

	Light();

	~Light() = default;
	void SetAmbient(DirectX::XMFLOAT4 colour);
	void SetDiffuse(DirectX::XMFLOAT4 colour);
	void SetDirection(DirectX::XMFLOAT3 dir);

	LightData GetLightData();
private:


	LightData mLightData;
};

