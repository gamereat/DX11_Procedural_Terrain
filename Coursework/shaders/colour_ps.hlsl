/* See the file "LICENSE" for the full license governing this code. */



struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
    
	float4 colour = float4(1.0, 1.0, 0.0, 1.0);

	return colour;
}