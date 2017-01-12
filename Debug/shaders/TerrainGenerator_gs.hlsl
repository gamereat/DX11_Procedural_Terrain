cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};
 
cbuffer PositionBuffer
{
	static float3 quad_position[4] =
	{
		float3(-1, 1, 0),
		float3(-1, -1, 0),
		float3(1, 1, 0),
		float3(1, -1, 0)
	};

	static float3 tri_position[3] =
	{
		float3(0.0, 1.0, 0.0),
		float3(-1.0, 0.0, 0.0),
		float3(1.0, 0.0, 0.0)
	};
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;

};

// pixel input type
struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

 
 
[maxvertexcount(3)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	input[0].position.w = 1.0f;

 
	for (int i = 0; i < 3; i++)
	{
		float3 vposition = tri_position[i] * 1;
		vposition = mul(vposition, (float3x3) worldMatrix) + input[0].position;
		output.position = mul(float4(vposition, 1.0), viewMatrix);
		output.position = mul(output.position, projectionMatrix);

		output.tex = input[0].tex;
		output.normal = input[0].normal;

		triStream.Append(output);
	}

 

}


