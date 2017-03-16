
cbuffer MatrixBuffer : register(cb0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};


cbuffer WaterBuffer : register(cb1)
{

    float amplutude;
    float time;
    float speed;
    float steepnesss;


    float3 freqancy;
    float padding;
};

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	uint id : SV_VertexID;


};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};



OutputType main(InputType input)
{
    OutputType output;
    input.position.w = 1.0f;


	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

 

    float3 normalPos;
    float length = sqrt(output.position.x * output.position.x + output.position.y * output.position.y + output.position.z * output.position.z);
    normalPos.y = output.position.y / length;
    normalPos.x = output.position.x / length;
    normalPos.z = output.position.z / length;

  
    
     float3 direction = float3(1, 1, 1) * freqancy ;
 
    // only simulate if wave is high than 0 
    if (freqancy.x > 0 && freqancy.y > 0 && freqancy.z >0 )
    {
        input.position += (float) (((steepnesss / direction * amplutude * 4) * amplutude) * direction *
         cos(float3(1, input.id + 1, 0) * (dot(direction, input.position.xyz)) + speed * time));


        input.normal += (float) (((steepnesss / direction * amplutude * 4) * amplutude) * direction * 
        cos(float3(1, input.id + 1, 0) * (dot(direction, input.position.xyz)) + speed * time));

    } 
	// Move the texture to half way along the screen

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
	output.normal = mul(input.normal, (float3x3)worldMatrix);

	// Normalize the normal vector.
	output.normal = normalize(output.normal);

	return output;

 

}
 