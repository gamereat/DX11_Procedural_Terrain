
/*
Standard martix Buffer
*/
cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

/*
* Buffer holding explosive effect settings
*/
cbuffer GeomentryBuffer : register(cb1)
{

    // current time of application
    float time;
    // gravity level of explotion
    float gravity;
    // force of explosive 
    float explosiveAmmount;
    // accelleration of explotion
    float explosiveAcceleration;

    // max time explotion can run
    float maxTime;

    //@DEPECATED
    //just used as padding now 
    float3 explotionMaximums;
};

  

struct InputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;

};

// pixel input type
struct OutputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    // holds how close the explotion is til done
    float explosionCompleteness : TEXCOORD1;
};


 
[maxvertexcount(4)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
    OutputType output;

    // make sure explotion hasn't  ended 
    float t = time;
    if (time >= maxTime)
    {
        t = maxTime;
    }

    // for for each vertex
    for (int i = 0; i < 3; i++)
    {
        input[i].position.w = 0;

        // work out normal cener 
        float4 normal = float4(((input[0].normal + input[1].normal + input[2].normal) / 3), 0);
 
        float3 vposition = input[i].position;;

        // find explosive effect
        // x = 1/2 *a * (t*t )+ v*t + x
        float3 explotionForce = normal.xyz *( (0.5 * explosiveAcceleration * t * t) + (explosiveAmmount * t));
        float3 gravityForce = float3(0, 1, 0) * 0.5 * gravity * t ;
        float3 explotionEffect = explotionForce + gravityForce;

        // apply explosive effect
          
        input[i].position.xyz += explotionEffect;
      
 
        vposition = mul(input[i].position.xyz, (float3x3) worldMatrix);
        output.position = mul(float4(vposition, 1.0), viewMatrix);
        output.position = mul(output.position, projectionMatrix);

        output.tex = input[i].tex;
        output.normal = input[i].normal;

        output.explosionCompleteness = t / maxTime;
        triStream.Append(output);
    }
     


}

 


