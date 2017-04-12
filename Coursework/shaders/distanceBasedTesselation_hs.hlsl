cbuffer TesselationBuffer : register(cb0)
{
  //  int4 outerTesselationValue;
   // int2 innerTesselationValue;

    float maxDistance;

    float minDistance;
   float minTesselationAmmount;
    float maxTesselationAmmount;

    float4 camPos;
};
cbuffer MatrixBuffer : register(cb1)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;

};
struct InputType
{
    float4 position : POSITION;

    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPosition[4] : TEXCOORD1;
    float3 lightPos[4] : TEXCOORD5;
    float3 position3D : TEXCOORD10;
    float3 viewDirection : TEXCOORD11;
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
    float4 position : POSITION;


    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 lightViewPosition[4] : TEXCOORD1;
    float3 lightPos[4] : TEXCOORD5;
    float3 position3D : TEXCOORD10;
    float3 viewDirection : TEXCOORD11;
};


 

ConstantOutputQuadType PatchConstantQuadFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputQuadType output;

	// Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = 1;
    output.edges[1] = 1;
    output.edges[2] =1;
    output.edges[3] = 1;


	// Set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = 1;
    output.inside[1] =1;


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
