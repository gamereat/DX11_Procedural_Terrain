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

 
};
struct ConstantOutputTriType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;

 
};

struct OutputType
{
    float4 position : POSITION;

    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPosition[4] : TEXCOORD1;
    float3 lightPos[4] : TEXCOORD5;
    float3 position3D : TEXCOORD10;
    float3 viewDirection : TEXCOORD11;
};



float getTessenationAmount(float4 pos);
ConstantOutputTriType PatchConstantTriFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputTriType output;

    //// Set the tessellation factors for the three edges of the triangle.
    //output.edges[0] = outerTesselationValue[0];
    //output.edges[1] = outerTesselationValue[1];
    //output.edges[2] = outerTesselationValue[2];


    //// Set the tessellation factor for tessallating inside the triangle.
    //output.inside = innerTesselationValue[0];


    return output;
}

ConstantOutputQuadType PatchConstantQuadFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputQuadType output;

 
    /// Get the center parts of the patch then find out tess amount from distnace from the cam
    output.edges[0] = getTessenationAmount((inputPatch[0].position + inputPatch[2].position) / 2);
    output.edges[1] = getTessenationAmount((inputPatch[0].position + inputPatch[1].position) / 2);
    output.edges[2] = getTessenationAmount((inputPatch[1].position + inputPatch[3].position) / 2);
    output.edges[3] = getTessenationAmount((inputPatch[2].position + inputPatch[3].position) / 2);
	
    output.inside[0] = getTessenationAmount((inputPatch[0].position + inputPatch[1].position + inputPatch[2].position + inputPatch[3].position) / 4);
    output.inside[1] = getTessenationAmount((inputPatch[0].position + inputPatch[1].position + inputPatch[2].position + inputPatch[3].position) / 4);

  //  output.edges[0] = outerTesselationValue[0];
//  output.edges[1] = outerTesselationValue[1];
 //   output.edges[2] = outerTesselationValue[2];
 // output.edges[3] = outerTesselationValue[3];
    
    
    /// Set the tessellation factor for tessallatin
  //  output.inside[0] = innerTesselationValue[0];
  //  output.inside[1] = innerTesselationValue[1];
    
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
	 


    output.lightPos = patch[0].lightPos;
    output.lightViewPosition = patch[0].lightViewPosition;
    
    output.position3D = patch[0].position3D;
    output.viewDirection = patch[0].viewDirection;
    return output;
}


float getTessenationAmount(float4 pos)
{
    // Get rid of padding value added ;
    float3 realCamPos = camPos.xyz;
    // find distance between current pos and the cam pos
    float distanc = distance(pos.xyz, realCamPos);

    // 
    float clampedVal = saturate((distanc - minDistance) / (maxDistance - minDistance));
    return lerp(maxTesselationAmmount, minTesselationAmmount, clampedVal) * lerp(maxTesselationAmmount, minTesselationAmmount, clampedVal);

}