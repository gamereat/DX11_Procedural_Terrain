// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 diffuseColour[4];
	float4 lightDirection[4];
	float4  ambientColour[4];
	float4 specularColour[4];
	float4 position[4];

	float4 specularPower[4];

	float4 constantAttenuationFactor[4];
	float4 linearAttenuationFactor[4];
	float4 quadraticAttenuationFactor[4];
	float4 range[4];

	int4 isSpecular[4];
	int4 isDirectionalLight[4];
	int4 isSpotLight[4];
	int4 isPointLight[4];
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 position3D : TEXCOORD2;

};

float4 main(InputType input) : SV_TARGET
{


	//

	float4 textureColour;
float3 lightDir;
float lightIntensity;
float4 colour;
float3 reflection;
float4 specular;
float4 finalSpec;
float attenuation;
float distance;

// set colour to default ambient colour
colour = ambientColour[0];

// Sample the pixel color from the texture using the sampler at this texture coordinate location.
textureColour = shaderTexture.Sample(SampleType, input.tex);

for (int i = 0; i < 4; i++)
{
	distance = length(input.position3D - position[i].xyz);

	if (distance < range[i].x || isDirectionalLight[i].x)
	{


		if (isPointLight[i].x)
		{
			// Invert the light direction for calculations.
			lightDir = normalize(input.position3D - position[i].xyz);

			// Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, -lightDir));

		}
		else if (isDirectionalLight[i].x)
		{

			// Invert the light direction for calculations.
			lightDir = -lightDirection[i].xyz;

			// Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, lightDir));
		}



		if (lightIntensity > 0.0f)
		{
			if (isPointLight[i].x)
			{
				// Work out the attenation value
				attenuation = 1 / (constantAttenuationFactor[i].x +
					linearAttenuationFactor[i].x * distance + pow(quadraticAttenuationFactor[i].x, 2));
			}


			colour += (diffuseColour[i] * lightIntensity);

			if (isPointLight[i].x)
			{
				colour = colour * attenuation;
			}
			colour = saturate(colour);

			if (isSpecular[i].x)
			{
				// Calculate reflection vector based on the light intensity, normal vector and light direction
				reflection = reflect(lightDir, input.normal);

				// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
				specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower[i]);

				//sum up specular light
				finalSpec = specularColour[i] * specular;

				// Add the specular component last to the output colour.
				colour = saturate(colour + finalSpec);
			}
		}
	}



}
// Multiply the texture pixel and the final diffuse color to get the final pixel color result.
colour = colour * textureColour;
return colour;

}

