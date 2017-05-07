// /* See the file "LICENSE" for the full license governing this code. */


Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


float4 main(InputType input) : SV_TARGET
{
    float4 textureColor = float4(0,0,0,0);

    float4 colour = float4(1.0f, 0.0f, 0.0f, 1);
    
    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = texture0.Sample(Sampler0, input.tex);

 

    float4 val = textureColor;
    float4 color = float4(1, 1, 1, 1);

// if they match, diff should be all 0
    float4 diff = val - color;

    if (any(diff))
    {

        }
    else
    {
        textureColor = float4(1, 1, 1, 0);

    }
    
    
    return textureColor;
}