// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

cbuffer CammeraBuffer : register(b1)
{
	float3 cammeraPostion;
	float padding;
}

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

};
cbuffer TimeBufferType : register(b2)
{
	float delatTime;
	float height;
	float frequancy;

	int  isSinWave;
	int  isCosWave;
	int  isThirdWave;
	int  isForthWave;
	int  isFithWave;

};
struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDirection : TEXCOORD1;
	float3 position3D : TEXCOORD2;


};

OutputType main(InputType input)
{
	OutputType output;
	float4 worldPosition;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	if (isSinWave == 1)
	{
		//offset position based on sine wave
		input.position.y += height * sin(input.position.x + delatTime * frequancy);

		//input.position.y = height * cos(input.position.x + delatTime * frequancy);
		//modify normals
		input.normal.x = 1 - height * cos(input.position.x + delatTime * frequancy);
		input.normal.y = abs(height * cos(input.position.x + delatTime * frequancy));


	}
	else if (isCosWave == 1)
	{
		//offset position based on sine wave
		input.position.y += height * cos(input.position.x + delatTime * frequancy);

		//input.position.y = height * cos(input.position.x + delatTime * frequancy);
		//modify normals
		input.normal.x = 1 - height * sin(input.position.x + delatTime * frequancy);
		input.normal.y = abs(height * sin(input.position.x + delatTime * frequancy));

	}
	else if (isThirdWave == 1)
	{

		////offset position based on sine wave
		input.position.y += height * input.position.x * sin((1 / input.position.x + delatTime) * frequancy);

		//input.position.y = height * cos(input.position.x + delatTime * frequancy);
		//modify normals
		input.normal.x = 1 - height * input.position.x * sin((1 / input.position.x + delatTime) * frequancy);
		input.normal.y = abs(height * input.position.x * sin((1 / input.position.x + delatTime) * frequancy));

	}
	else if (isForthWave == 1)
	{

		////offset position based on sine wave
		input.position.y += height * sin(input.position.x + (delatTime * 3)  * frequancy);
		input.position.x += height * sin(input.position.x + (delatTime * 2)  * frequancy);

		//input.position.y = height * cos(input.position.x + delatTime * frequancy);
		//modify normals
		input.normal.x = 1 - height * sin((1 / input.position.x + (delatTime * 3)) * frequancy);
		input.normal.y = abs(height * sin((1 / input.position.x + (delatTime * 2)) * frequancy));

	}
	else if (isFithWave == 1)
	{

		////offset position based on sine wave
		input.position.y += height * sin((input.position.x + delatTime) * frequancy) * sin(((input.position.x + delatTime) * 2)* frequancy);

		//input.position.y = height * cos(input.position.x + delatTime * frequancy);
		//modify normals
		input.normal.x = 1 - height * sin((1 / input.position.x + (delatTime * 3)) * frequancy);
		input.normal.y = abs(height * sin((1 / input.position.x + (delatTime * 2)) * frequancy));

	}
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

	// Calculate the position of the vertex in the world.
	worldPosition = mul(input.position, worldMatrix);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
	output.viewDirection = cammeraPostion.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
	output.viewDirection = normalize(output.viewDirection);

	// world position of vertex
	output.position3D = mul(input.position, worldMatrix);

	return output;
}