// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader

cbuffer ScreenSizeBuffer : register(b1)
{
	float screenWidth;
	float screenHeight;
	float2 padding;
}

cbuffer MatrixBuffer : register(b0)
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
};

struct OutputType
{
    float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCords1 : TEXCOORD1;
	float2 texCords2 : TEXCOORD2;
	float2 texCords3 : TEXCOORD3;
	float2 texCords4 : TEXCOORD4;
	float2 texCords5 : TEXCOORD5;
	float2 texCords6 : TEXCOORD6;
	float2 texCords7 : TEXCOORD7;
	float2 texCords8 : TEXCOORD8;



};

OutputType main(InputType input)
{
    OutputType output;
	float texelWidth, texelHeight;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific width.
	texelWidth = 1.0f / screenWidth;
	texelHeight = 1.0f / screenHeight;

	output.texCords1 = input.tex + float2(-texelWidth, -texelHeight);
	output.texCords2 = input.tex + float2(0.0f, texelHeight);
	output.texCords3 = input.tex + float2(texelWidth, -texelHeight);
	output.texCords4 = input.tex + float2(-texelWidth, 0.0f);
	output.texCords5 = input.tex + float2(texelWidth, 0.0f);
	output.texCords6 = input.tex + float2(-texelWidth, texelHeight);
	output.texCords7 = input.tex + float2(0.0f, texelHeight);
	output.texCords8 = input.tex + float2(texelWidth, texelHeight);

    return output;
}