// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry


Texture2D defaultTexture : register(t0);
Texture2D lowTexture : register(t1);
Texture2D mediumTexture : register(t2);
Texture2D highTexture : register(t3);

Texture2D depthMapTexture : register(t4);
Texture2D depthMapTexture1 : register(t5);
Texture2D depthMapTexture2 : register(t6);
Texture2D depthMapTexture3 : register(t7);

SamplerState SampleTypeWrap : register(s0);
SamplerState SampleTypeClamp : register(s1);

cbuffer LightBuffer : register(cb0)
{
    float4 diffuseColour[4];
    float4 lightDirection[4];
    float4 ambientColour[4];
    float4 specularColour[4];
    float4 position[4];

    float4 specularPower[4];

	// 0 is constant
	// 1 linear
	// 2 quadratic
	// 3 range
    float4 attenuationValues[4];

	//0 - directional
	//1- spot
	//2- point
    int4 lightType[4];

    int isSpecular[4];

	//if it will generate a shadow 
    int willGenerateShadows[4];
};

cbuffer TerrainSettingTextureBuffer : register(cb1)
 {
    int displayNormalMap;

    int3 padding;
};
 
struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 lightViewPosition[4] : TEXCOORD1;
    float3 lightPos[4] : TEXCOORD5;
    float3 position3D : TEXCOORD10;
    float3 viewDirection : TEXCOORD11;
};
 
float4 main(InputType input) : SV_TARGET
{
	 
    // If Needing to just display the normal map 
    if (displayNormalMap)
    {
    // Gets the input normal map then returns colour based off it 
        return float4(1, 1, 1, 0) - float4(input.normal, 1);;
    }
 
    float bias;
    float4 shadowColourValue[4];
    float4 color = float4(0, 0, 0, 0);
    float2 projectTexCoord[4];
    float depthValue = 0.0f;
    float4 textureColor = float4(0, 0, 0, 0);
    float attenuation = 0;
    float distance = 0;
    float3 lightDir = float3(0, 0, 0);
    float4 specular = float4(0, 0, 0, 0);
    float3 reflection = float3(0, 0, 0);
    float4 finalSpec = float4(0, 0, 0, 0);

   // Set the bias value for fixing the floating point precision issues.
    bias = 0.0001f;

	// Set the default output color to the ambient light value for all pixels.
    
    //color = ambientColour[0];

    for (int i = 0; i < 4; i++)
    {
 
        shadowColourValue[i] = float4(0, 0, 0, 0.0f);
  
        projectTexCoord[i] = float2(0, 0);
    }

	// loop for each light
    for (int i = 0; i < 4; i++)
    {

        float lightIntensity = 0.0f;
        float lightDepthValue = 0.0f;
        float depthValue = 0.0f;

		// Calculate the projected texture coordinates.
		
        projectTexCoord[i].x = input.lightViewPosition[i].x / input.lightViewPosition[i].w / 2.0f + 0.5f;
        projectTexCoord[i].y = -input.lightViewPosition[i].y / input.lightViewPosition[i].w / 2.0f + 0.5f;


	
		// Determine if the projected coordinates are in the 0 to 1 range.  If so then this pixel is in the view of the light.
		
   
        if ((saturate(projectTexCoord[i].x) == projectTexCoord[i].x) && (saturate(projectTexCoord[i].y) == projectTexCoord[i].y))
        {

            // work out which depth texture to use
            switch (i)
            {
                case 0:
                    depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord[i]).r;

                    break;
                case 1:
                    depthValue = depthMapTexture1.Sample(SampleTypeClamp, projectTexCoord[i]).r;

                    break;
                case 2:
                    depthValue = depthMapTexture2.Sample(SampleTypeClamp, projectTexCoord[i]).r;

                    break;
                case 3:
                    depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord[i]).r;

                    break;
                default:
                    depthValue = depthMapTexture3.Sample(SampleTypeClamp, projectTexCoord[i]).r;

                    break;
            }
			// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.

			// Calculate the depth of the light.
            lightDepthValue = input.lightViewPosition[i].z / input.lightViewPosition[i].w;

			// Subtract the bias from the lightDepthValue.
            lightDepthValue = lightDepthValue - bias;

			// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
			// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
            if (lightDepthValue < depthValue)
            {

                distance = length(input.position3D - position[i].xyz);

                if (distance < attenuationValues[i].w || lightType[i].x == 1)
                {
                    
				 //Calculate the amount of light on this pixel.
                    if (lightType[i].y == 1)
                    {
					// Invert the light direction for calculations.
                        lightDir = normalize(input.position3D - position[i].xyz);

					// Calculate the amount of light on this pixel.
                        lightIntensity = saturate(dot(input.normal, -lightDir));

                    }
                    else if (lightType[i].x == 1)
                    {

					// Invert the light direction for calculations.
                        lightDir = -lightDirection[i].xyz;

					// Calculate the amount of light on this pixel.
                        lightIntensity = saturate(dot(input.normal, lightDir));
                    }
                    if (lightIntensity > 0.0f)
                    {
                    
                        if (lightType[i].y == 1)
                        {
						// Work out the attenation value
                            attenuation = 1 / (attenuationValues[i].x +
							attenuationValues[i].y * distance + pow(attenuationValues[i].z, 2));
                        }

                        // add defuse colour to shadow
                   
                        shadowColourValue[i] = (diffuseColour[i] * lightIntensity);

                        if (lightType[i].y == 1)
                        {
                     
                            shadowColourValue[i] = shadowColourValue[i] * attenuation;
                        }
 
                        if (isSpecular[i] == 1)
                        {
						// Calculate reflection vector based on the light intensity, normal vector and light direction
                            reflection = reflect(lightDir, input.normal);

						// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
                            specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower[i]);

						//sum up specular light
                            finalSpec = specularColour[i] * specular;

						// Add the specular component last to the output colour.
                       
                            shadowColourValue[i] = saturate(shadowColourValue[i] + finalSpec);
                   
                      
                        }
                      
                  
                  

              
                    }
             
           
                }
            }

   

              
              
        }
        else
        {
            distance = length(input.position3D - position[i].xyz);

            if (distance < attenuationValues[i].w || lightType[i].x == 1)
            {
                if (lightType[i].y == 1)
                {
					// Invert the light direction for calculations.
                    lightDir = normalize(input.position3D - position[i].xyz);

					// Calculate the amount of light on this pixel.
                    lightIntensity = saturate(dot(input.normal, -lightDir));

                }
                else if (lightType[i].x == 1)
                {

					// Invert the light direction for calculations.
                    lightDir = -lightDirection[i].xyz;

					// Calculate the amount of light on this pixel.
                    lightIntensity = saturate(dot(input.normal, lightDir));
                }



                if (lightIntensity > 0.0f)
                {
                    if (lightType[i].y == 1)
                    {
						// Work out the attenation value
                        attenuation = 1 / (attenuationValues[i].x +
							attenuationValues[i].y * distance + pow(attenuationValues[i].z, 2));
                    }


                    shadowColourValue[i] += (diffuseColour[i] * lightIntensity);

                    if (lightType[i].y == 1)
                    {
     
                        shadowColourValue[i] = shadowColourValue[i] * attenuation;
                    }
                    

                    if (isSpecular[i] == 1)
                    {
						// Calculate reflection vector based on the light intensity, normal vector and light direction
                        reflection = reflect(lightDir, input.normal);

						// Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.
                        specular = pow(saturate(dot(reflection, input.viewDirection)), specularPower[i]);

						//sum up specular light
                        finalSpec = specularColour[i] * specular;

						// Add the specular component last to the output colour.
                        shadowColourValue[i] = saturate(shadowColourValue[i] + finalSpec);
                    }
                }
				
            }


        }
 
        // make sure colour is kept within 0 and 1
        color = saturate(color + shadowColourValue[i]);

    }
   
	color = saturate(color);
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
 //   textureColor = texture0.Sample(SampleTypeWrap, input.tex);
   
    float4 lowTexColour = lowTexture.Sample(SampleTypeWrap, input.tex);

    float4 mediumTexColour = mediumTexture.Sample(SampleTypeWrap, input.tex);

    float4 highTexColour = highTexture.Sample(SampleTypeWrap, input.tex);

    // Calculate the slope of this point.
    float slope = 1.0f - input.normal.y;
    float blendAmount;

    // Determine which texture to use based on height.
    if (slope < 0.2)
    {
        blendAmount = slope / 0.2f;


    
        textureColor = lerp(mediumTexColour, lowTexColour, blendAmount);

    }else  if ((slope < 0.7) && (slope >= 0.2f))
    {
        blendAmount = (slope - 0.2f) * (1.0f / (0.7f - 0.2f));
    
        textureColor = lerp(mediumTexColour, highTexColour, blendAmount);


    }else    if (slope >= 0.7)
    {
    
        textureColor = lowTexColour;
 
    }else
    {
        textureColor = float4(0, 1, 0, 1);

    }

	float4 depth= depthMapTexture.Sample(SampleTypeWrap, input.tex);
    depth.w = 1;
	// Combine the light and texture color.
    color = color * textureColor;
    return color;

  
 }