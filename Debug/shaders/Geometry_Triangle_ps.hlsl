Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float explosionCompleteness : TEXCOORD1;

};


float4 main(InputType input) : SV_TARGET
{
    float4 colour = lerp(texture0.Sample(Sampler0, input.tex), float4(0.5, 0, 0, 1) * texture0.Sample(Sampler0, input.tex), input.explosionCompleteness);
    return saturate( colour);
}