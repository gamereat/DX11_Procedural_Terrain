 
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix[4];
    matrix lightProjectionMatrix[4];
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
    float4 position : POSITION;

    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 lightViewPosition[4] : TEXCOORD1;
    float3 lightPos[4] : TEXCOORD5;
    float3 position3D : TEXCOORD10;
    float3 viewDirection : TEXCOORD11;
};

struct OutputType
{
    float4 position : SV_POSITION;

    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPosition[4] : TEXCOORD1;
    float3 lightPos[4] : TEXCOORD5;
    float3 position3D : TEXCOORD10;
    float3 viewDirection : TEXCOORD11;
};
 // Function used use to lerp
float2 tesselationLept(float2 cords1, float2 cords2, float2 cords3, float2 cords4, float2 uvCord);
float4 tesselationLept(float4 cords1, float4 cords2, float4 cords3, float4 cords4 , float2 uvCord);
float3 tesselationLept(float3 cords1, float3 cords2, float3 cords3, float3 cords4, float2 uvCord);


[domain("tri")]
OutputType main(ConstantOutputTriType input, float3 bary : SV_DomainLocation, const OutputPatch<InputType, 3> patch)
{
    float4 vertexPosition;
	float3 vertexTexture;
	OutputType output;
	 
	// Determine the position of the new vertex.
    vertexPosition = bary.x * patch[0].position + bary.y * patch[1].position + bary.z * patch[2].position;


   // vertexPosition = tesselationLept(patch[0].position, patch[1].position, patch[2].position, patch[3].position, uvwCoord);
	// Calculate the position of the new vertex against the world, view, and projection matrices.
	output.position = mul(vertexPosition, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);


 
    // pass though outputs
    output.tex = bary.x * patch[0].tex + bary.y * patch[1].tex + bary.z * patch[2].tex;
    output.normal = bary.x * patch[0].normal + bary.y * patch[1].normal + bary.z * patch[2].normal;
    output.position3D = bary.x * patch[0].position3D + bary.y * patch[1].position3D + bary.z * patch[2].position3D;
    output.viewDirection = bary.x * patch[0].viewDirection + bary.y * patch[1].viewDirection + bary.z * patch[2].viewDirection;
     output.lightPos = patch[0].lightPos;
    output.lightViewPosition = patch[0].lightViewPosition;
  

	return output;

}
 

  
float2 tesselationLept(float2 cords1, float2 cords2, float2 cords3, float2 cords4, float2 uvCord)
{

	// Determine the tex coords of shape
    float2 n1 = lerp(cords1, cords2, uvCord.x);
    float2 n2 = lerp(cords4, cords3, uvCord.x);
    return lerp(n1, n2, uvCord.y);

}

float3 tesselationLept(float3 cords1, float3 cords2, float3 cords3, float3 cords4, float2 uvCord)
{

	// Determine the tex coords of shape
    float3 n1 = lerp(cords1, cords2, uvCord.x);
    float3 n2 = lerp(cords4, cords3, uvCord.x);


    return lerp(n1, n2, uvCord.y);
}
float4 tesselationLept(float4 cords1, float4 cords2, float4 cords3, float4 cords4, float2 uvCord)
{

	// Determine the tex coords of shape
    float4 n1 = lerp(cords1, cords2, uvCord.x);
    float4 n2 = lerp(cords4, cords3, uvCord.x);
    return lerp(n1, n2, uvCord.y);

}