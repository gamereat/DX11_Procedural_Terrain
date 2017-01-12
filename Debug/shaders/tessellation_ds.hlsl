// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct ConstantOutputQuadType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};
struct ConstantOutputTriType
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;

	float3 normal : NORMAL;
	float2 tex : TEXCOORD;
};

struct OutputType
{
    float4 position : SV_POSITION;

	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

[domain("tri")]
OutputType triEnty(ConstantOutputTriType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 3> patch)
{
    float3 vertexPosition;
    OutputType output;
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
	vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
    

	output.tex = uvwCoord.x * patch[0].tex + uvwCoord.y * patch[1].tex + uvwCoord.z * patch[2].tex;
	output.normal = uvwCoord.x * patch[0].normal + uvwCoord.y * patch[1].normal + uvwCoord.z * patch[2].normal;


    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
 

    return output;
}

//
//
//[domain("quad")]
//OutputType quadEnty(ConstantOutputQuadType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
//{
//	float3 vertexPosition;
//	OutputType output;
//	 
//	// Determine the position of the new vertex.
//	float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.x);
//	float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.x);
//	vertexPosition = lerp(v1, v2, uvwCoord.y);
//
//
//	// Calculate the position of the new vertex against the world, view, and projection matrices.
//	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
//	output.position = mul(output.position, viewMatrix);
//	output.position = mul(output.position, projectionMatrix);
//
//	output.tex = patch[0].tex;
//
//
//	return output;
//}
// 
