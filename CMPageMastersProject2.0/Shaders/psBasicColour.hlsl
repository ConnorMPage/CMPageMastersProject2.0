SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColour;
    float4 diffuseColour;
    float3 lightDirection;
    float padding;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float3 lightDir;
    float lightIntensity;
    float4 colour;


	// Set the default output colour to the ambient light value for all pixels.
    colour = ambientColour;

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
       
        colour += (diffuseColour * lightIntensity);
    }

    
    colour = saturate(colour);

    return colour;
}
