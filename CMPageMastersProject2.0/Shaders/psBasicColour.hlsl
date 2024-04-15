SamplerState SampleType;

cbuffer LightBuffer
{
    float4 ambientColor;
    float4 diffuseColor;
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
    float4 color;


	// Set the default output color to the ambient light value for all pixels.
    color = ambientColor;

	// Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    lightIntensity = saturate(dot(input.normal, lightDir));

    if (lightIntensity > 0.0f)
    {
       
        color += (diffuseColor * lightIntensity);
    }

    
    color = saturate(color);

    return color;
}
