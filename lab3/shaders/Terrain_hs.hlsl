// Tessellation Hull Shader
// Prepares control points for tessellation

 

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
    output.edges[0] = 2;
    output.edges[1] = 2;
    output.edges[2] = 2;


    // Set the tessellation factor for tessallating inside the triangle.
    output.inside = 2;


    return output;
}

ConstantOutputQuadType PatchConstantQuadFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputQuadType output;

	// Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = 2;
    output.edges[1] = 2;
    output.edges[2] = 2;
    output.edges[3] = 2;


	// Set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = 2;
    output.inside[1] = 2;


    return output;
}

 
 


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantQuadFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;

	// Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    output.normal = patch[pointId].normal;
    output.tex = patch[pointId].tex;
	 
    return output;
}

 