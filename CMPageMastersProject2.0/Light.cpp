#include "Light.h"

Light::Light()
{
}

void Light::SetAmbient(DirectX::XMFLOAT4 colour)
{
	mLightData.AmbientColour = colour;
}

void Light::SetDiffuse(DirectX::XMFLOAT4 colour)
{
	mLightData.DiffuseColour = colour;
}

void Light::SetDirection(DirectX::XMFLOAT3 dir)
{
	mLightData.Direction = dir;
}

Light::LightData Light::GetLightData()
{
	return mLightData;
}
