// Tessellation Hull Shader
// Prepares control points for tessellation


cbuffer TesselationBuffer : register(cb0)
{
    int4 outerTesselationValue;
    int2 innerTesselationValue;
    int2 padding;
};

struct InputType
{
    float3 position : POSITION;

    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct ConstantOutputQuadType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;

    float2 uvPos[4] : TEXCOORD;

};
struct ConstantOutputTriType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;

    float2 uvPos[3] : TEXCOORD;

};

struct OutputType
{
    float3 position : POSITION;


    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
};

ConstantOutputTriType PatchConstantTriFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputTriType output;

    // Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = outerTesselationValue[0];
    output.edges[1] = outerTesselationValue[1];
    output.edges[2] = outerTesselationValue[2];


    // Set the tessellation factor for tessallating inside the triangle.
    output.inside = innerTesselationValue[0];


    return output;
}

ConstantOutputQuadType PatchConstantQuadFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputQuadType output;

	// Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = outerTesselationValue[0];
    output.edges[1] = outerTesselationValue[1];
    output.edges[2] = outerTesselationValue[2];
    output.edges[3] = outerTesselationValue[3];


	// Set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = innerTesselationValue[0];
    output.inside[1] = innerTesselationValue[1];


    return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantTriFunction")]
OutputType triEnty(InputPatch<InputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

	// Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    output.normal = patch[pointId].normal;
    output.tex = patch[pointId].tex;
	 
    return output;
}



// 
//
//
//[domain("quad")]
//[partitioning("integer")]
//[outputtopology("triangle_cw")]
//[outputcontrolpoints(4)]
//[patchconstantfunc("PatchConstantQuadFunction")]
//OutputType quadEnty(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
//{
//	OutputType output;
//
//	// Set the position for this control point as the output position.
//	output.position = patch[pointId].position;
//
//
//
//
//	// Set the input color as the output color.
//	output.colour = patch[pointId].colour;
//
//	return output;
//}

 