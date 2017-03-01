// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry


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
    float4 textureColor;

    float4 colour = float4(1.0f, 0.0f, 0.0f, 1);
    float4 lerpAmmount = float4(0.5f, 0.0f, 0.0f, 1);
     // Calculate the slope of this point.
    float slope = 1.0f - input.normal.y;
    float blendAmount;

      // Calculate the slope of this point.
    slope = 1.0f - input.normal.y;
 
    // Determine which texture to use based on height.
    if(slope < 0.2)
    {
        blendAmount = slope / 0.2f;
        colour= float4(1,0,0,1);
    }
	
    if((slope < 0.7) && (slope >= 0.2f))
    {
        blendAmount = (slope - 0.2f) * (1.0f / (0.7f - 0.2f));
        colour = float4(0, 1, 0,1);
     }

    if(slope >= 0.7) 
    {
      //  colour = float4(0, 0, 1, 1);
        
     }

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = texture0.Sample(Sampler0, input.tex);

    return colour;
}