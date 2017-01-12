Texture2D shaderTexture : register(t0);
 SamplerState SampleType : register(s0);
 

cbuffer ScreenSizeBuffer : register(cb0)
{
	float3 rgbDistortLevel;
	float totalTime;

	float3 freqancy;
	float padding1;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;


};

float4 main(InputType input) : SV_TARGET
{
    float4 colour;



	// samples the texture offset but the total time the application have been running
	// Texture will wrap 
	float disR = shaderTexture.Sample(SampleType, float2((input.tex[0] - totalTime) * freqancy.x, 0.5)).r;
	float disG = shaderTexture.Sample(SampleType, float2((input.tex[0] - totalTime) * freqancy.y, 0.5)).g;
	float disB = shaderTexture.Sample(SampleType, float2((input.tex[0] - totalTime) * freqancy.z, 0.5)).b;
	float3 chro = rgbDistortLevel * float3(disR, disG, disB);

 
 	
	float4 rColour = shaderTexture.Sample(SampleType,	float2( input.tex[0] + chro.x , input.tex[1]),0);
	float4 gColour = shaderTexture.Sample(SampleType, float2(input.tex[0] + chro.y, input.tex[1]),0);
	float4 bColour = shaderTexture.Sample(SampleType, float2(input.tex[0] + chro.z, input.tex[1]),0);
	// Initialize the colour to black.
   colour = float4(rColour.r , gColour.g, bColour.b, 1.0f);

 
    return colour;
}
