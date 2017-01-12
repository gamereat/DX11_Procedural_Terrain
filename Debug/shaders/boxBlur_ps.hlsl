// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);


struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;


};

float4 main(InputType input) : SV_TARGET
{
	float4 colour;

	colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine surrounding pixels to the colour.
	colour += shaderTexture.Sample(SampleType, input.tex);
	colour += shaderTexture.Sample(SampleType, input.texCoord1);
	colour += shaderTexture.Sample(SampleType, input.texCoord2);
	colour += shaderTexture.Sample(SampleType, input.texCoord3);
	colour += shaderTexture.Sample(SampleType, input.texCoord4);
	colour += shaderTexture.Sample(SampleType, input.texCoord5);
	colour += shaderTexture.Sample(SampleType, input.texCoord6);
	colour += shaderTexture.Sample(SampleType, input.texCoord7);
	colour += shaderTexture.Sample(SampleType, input.texCoord8);

	colour /= 9.0f;

	// Set the alpha channel to one.
	colour.a = 1.0f;

	return colour;


}

