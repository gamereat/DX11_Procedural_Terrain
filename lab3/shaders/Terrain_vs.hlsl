// texture vertex shader
// Basic shader for rendering textured geometry

cbuffer MatrixBuffer : register(cb0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    matrix lightViewMatrix[4];
    matrix lightProjectionMatrix[4];
};
cbuffer CammeraBuffer : register(cb1)
{
    float3 cammeraPostion;
    float padding;
}
cbuffer LightBuffer2 : register(cb2)
{
    float4 lightPosition[4];
};

cbuffer TerrainBuffer : register(cb3)
{
    /*
    0. FaultLineDisplacement,
	1. PerlinNoise,
	2. FractionalBrowningNoise,
	3. SimplexNoise,
	4. DiamondSquare,
	5. RandomNoise,
	*/
    int terrainGenerationType;

    float highScale;
    
    int2 padding2;
}

cbuffer FaltLineDisplacementBuffer : register(cb4)
{
    /*
    Number of iterations to complete
    */
    int numberOfIterations;

    /*
    The starting displacment value 
    */
    float startingDisplacement;

    /*
    The minimum value the last displacement will set to be 
    */
    float minimumDisplacement;

    /*
    SmoothingValue
    */
    float smoothingValue;
    /*

    2D array holding the diffrent random points on the surface which should be sued to complete fault line displ


    Using heavy packing on varrible. 
    x and y being the first 2 random points on the surface,
    z and w beingthe second random point on the surface
    Note 1000 will be the maxium number of iterations

    */
    int4 interationsRandomPoints[2500];

    /*
    IF fault line displacement is active
    */
    int enableFaultLineDisplacement;

    float3 padding3;

}

cbuffer FractionalBrowningNoiseBuffer : register(cb5)
{
	float fbnFrequancy;
	float fbnOctaves;
	float fbnAmplitude;
	float fbnLacunarity;
	float fbnPersistence;
	float fbnPelinNoiseFreqnacy;
	float heightScale;
	float fbnPadding;
	
 }


cbuffer dimondSquareBuffer : register(cb6)
{
	int widthOfGrid;
	int heightOfGrid;

	int2 dimondSquarePadding;

	float topRightRandomNumber;
	float topLeftRandomNumber;
	float bottomRightRandomNumber;
	float bottomLeftRandomNumber;
};
struct dimondSquareResult
{
    bool found ;
    float topLeft;
    float topRight;
    float bottomLeft;
    float bottomRight;
    float result;

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
    float4 lightViewPosition[4] : TEXCOORD1;
    float3 lightPos[4] : TEXCOORD5;
    float3 position3D : TEXCOORD10;
    float3 viewDirection : TEXCOORD11;
};
dimondSquareResult DiamondSquare(int x, int y, float height, float width, float topLeft, float topRight, float bottomLeft, float bottomRight);
float adverage(float a, float b);
float adverage(float a, float b, float c, float d);
float FractionalBrownianMotion(float x, float y, int octaves, float frequency, float amplitude, float pelinFrequancy);
float grad(int hash, float x, float y);
float grad(int hash, float x);
float PelinNoise(float x, float y, float freqancy);
int fastfloor(float fp);
float FaultLineDisplacement(float x, float z);
float FaultLineDisplacement(float2 xz);
float3 Smoothing(float smoothingValue, float3 position, float3 pointAbove, float3 pointBellow, float3 pointLeft, float3 pointRight);
float3 CaculateNormalMapFaultLineDisplacement(float3 position);
float FaultLineDisplacementSmoothing(int sizeOfnNeighboursSmoothing, float3 position);

OutputType main(InputType input)
{
    OutputType output;
    float4 worldPosition;
    

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
 

	switch (terrainGenerationType)
	{
	
	    // FAULT LINE DISPLACEMENT
	    case 0:
	    {	
		    input.position.y = FaultLineDisplacement(input.position.x, input.position.z);
		    output.normal = CaculateNormalMapFaultLineDisplacement(input.position.xyz);
		    break;
	    }
	    //PELIN NOISE
	    case 1:
	    {
		    break;
	    }
	    // FractionalBrowningNoise
	    case 2:
	    {
			
		    input.position.y = FractionalBrownianMotion(input.position.x, input.position.z, fbnOctaves, fbnFrequancy, fbnAmplitude, fbnPelinNoiseFreqnacy) * heightScale;

		
		    break;
	    }
	    // SimplexNoise 
	    case 3:
	    {
	    //	input.position.y = PelinNoise(input.position.x, input.position.z,1);
		    break;
	    }




	}
	 
	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
     

    for (int i = 0; i < 4; i++)
    {
		// Calculate the position of the vertice as viewed by the light source.
        output.lightViewPosition[i] = mul(input.position, worldMatrix);
        output.lightViewPosition[i] = mul(output.lightViewPosition[i], lightViewMatrix[i]);
        output.lightViewPosition[i] = mul(output.lightViewPosition[i], lightProjectionMatrix[i]);
    }
	// Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    

    // Calculate the position of the vertex in the world.
    worldPosition = mul(input.position, worldMatrix);

	// Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
  
    output.viewDirection = cammeraPostion.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);

	// world position of vertex
    output.position3D = mul(input.position, worldMatrix);

    for (int i = 0; i < 4; i++)
    {
		// Determine the light position based on the position of the light and the position of the vertex in the world.
        output.lightPos[i] = lightPosition[i].xyz - worldPosition.xyz;

		// Normalize the light position vector.
        output.lightPos[i] = normalize(output.lightPos[i]);
    }

    if(terrainGenerationType != 0)
    {
          	// Calculate the normal vector against the world matrix only.
        output.normal = mul(input.normal, (float3x3) worldMatrix);
	
           // Normalize the normal vector.
        output.normal = normalize(output.normal);
    }
    return output;
}
 
float FaultLineDisplacementSmoothing(int sizeOfnNeighboursSmoothing, float3 position)
{
    float weight0, weight1, weight2;

    // guassain blur imp
    	// Create the weights that each neighbor pixel will contribute to the blur.
    weight0 = 0.4062f;
    weight1 = 0.2442f;
    weight2 = 0.0545f;

    float result = position;
    float3 offset = float3(1.0, 1.0, 0.0);
    float left = FaultLineDisplacement(position.xz - offset.xz);
    float right = FaultLineDisplacement(position.xz + offset.xz);
    float bellow = FaultLineDisplacement(position.xz - offset.zy);
    float under = FaultLineDisplacement(position.xz + offset.zy);
     
    return result; 
 

}

float3 CaculateNormalMapFaultLineDisplacement(float3 position)
{
    float3 normal;

    float3 offset = float3(1.0, 1.0, 0.0);
    float left = FaultLineDisplacement(position.xz - offset.xz);
    float right = FaultLineDisplacement(position.xz + offset.xz);
    float bellow = FaultLineDisplacement(position.xz - offset.zy);
    float under = FaultLineDisplacement(position.xz + offset.zy);

    normal.x = left - right;
    normal.y = bellow - under;
    normal.z = 2.0;
    return normal = normalize(normal);
    
}

float FaultLineDisplacement(float2 xz)
{
    return FaultLineDisplacement(xz.x, xz.y);

}

float FaultLineDisplacement(float x, float z)
{
     
    float yAxisChange = 0;


    float displacement = startingDisplacement;
    for (int iter = 0; iter < numberOfIterations; iter++)
    {



        float2 randPoint1, randPoint2;
        
        randPoint1 = float2((float) interationsRandomPoints[iter].x, (float) interationsRandomPoints[iter].y);
        randPoint2 = float2((float) interationsRandomPoints[iter].z, (float) interationsRandomPoints[iter].w);


        float a = (float) ((float) randPoint2.y - (float) randPoint1.y);
        float b = (float) -((float) randPoint2.x - (float) randPoint1.x);
        float c = (float) (-(float) randPoint1.x * ((float) randPoint2.y - (float) randPoint1.y)) + ((float) randPoint1.y * ((float) randPoint2.x - (float) randPoint1.x));
  


        if (((a * z) + (b * x) - c) > 0)
        {
            yAxisChange += displacement;
        }
        else
        {
            yAxisChange -= displacement;
        }

       
        if (iter < numberOfIterations)
        {
            displacement = startingDisplacement - ((float) iter / (float) numberOfIterations) * ((float) displacement - (float) minimumDisplacement);
        }
        else
        {
            displacement = startingDisplacement;
        }
    }




    return yAxisChange;

}

float PelinNoise(float x, float y, float freqancy)
{

	int perm[256] = {
		151, 160, 137, 91, 90, 15,
		131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
		190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
		88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
		77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
		102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
		135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
		5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
		223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
		129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
		251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
		49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
		138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
	};
	x *= freqancy;
	y *= freqancy;
	float n0, n1, n2;   // Noise contributions from the three corners

						// Skewing/Unskewing factors for 2D
	const float F2 = 0.366025403f;  // F2 = (sqrt(3) - 1) / 2
	const float G2 = 0.211324865f;  // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

									// Skew the input space to determine which simplex cell we're in
	float s = (x + y) * F2;  // Hairy factor for 2D
	float xs = x + s;
	float ys = y + s;
	int i = fastfloor(xs);
	int j = fastfloor(ys);

	// Unskew the cell origin back to (x,y) space
	float t = (float)(i + j) * G2;
	float X0 = i - t;
	float Y0 = j - t;
	float x0 = x - X0;  // The x,y distances from the cell origin
	float y0 = y - Y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
	int i1, j1;  // Offsets for second (middle) corner of simplex in (i,j) coords
	if (x0 > y0) {   // lower triangle, XY order: (0,0)->(1,0)->(1,1)
		i1 = 1;
		j1 = 0;
	}
	else {   // upper triangle, YX order: (0,0)->(0,1)->(1,1)
		i1 = 0;
		j1 = 1;
	}

	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6

	float x1 = x0 - i1 + G2;            // Offsets for middle corner in (x,y) unskewed coords
	float y1 = y0 - j1 + G2;
	float x2 = x0 - 1.0f + 2.0f * G2;   // Offsets for last corner in (x,y) unskewed coords
	float y2 = y0 - 1.0f + 2.0f * G2;

	// Calculate the contribution from the first corner
	float t0 = 0.5f - x0*x0 - y0*y0;
	if (t0 < 0.0f) {
		n0 = 0.0f;
	}
	else {
		t0 *= t0;
		n0 = t0 * t0 * grad(perm[i + perm[j]], x0, y0);
	}

	// Calculate the contribution from the second corner
	float t1 = 0.5f - x1*x1 - y1*y1;
	if (t1 < 0.0f) {
		n1 = 0.0f;
	}
	else {
		t1 *= t1;
		n1 = t1 * t1 * grad(perm[i + i1 + perm[j + j1]], x1, y1);
	}

	// Calculate the contribution from the third corner
	float t2 = 0.5f - x2*x2 - y2*y2;
	if (t2 < 0.0f) {
		n2 = 0.0f;
	}
	else {
		t2 *= t2;
		n2 = t2 * t2 * grad(perm[i + 1 + perm[j + 1]], x2, y2);
	}

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
	return 45.23065f * (n0 + n1 + n2);
}

int fastfloor(float fp)
{
	int i = (int)fp;
	return (fp < i) ? (i - 1) : (i);
}

float grad(int hash, float x) {
	int h = hash & 0x0F;        // Convert low 4 bits of hash code
	float grad = 1.0f + (h & 7);    // Gradient value 1.0, 2.0, ..., 8.0
	if ((h & 8) != 0) grad = -grad; // Set a random sign for the gradient
									//  float grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above
	return (grad * x);              // Multiply the gradient with the distance
}

float grad(int hash, float x, float y) {
	int h = hash & 0x3F;  // Convert low 3 bits of hash code
	float u = h < 4 ? x : y;  // into 8 simple gradient directions,
	float v = h < 4 ? y : x;  // and compute the dot product with (x,y).
	return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f*v : 2.0f*v);
}
 
float FractionalBrownianMotion(float x, float y, int octaves, float frequency, float amplitude,  float pelinFrequancy)
{
	float output = 0.f;
	float denom = 0.f;
 
	for (int i = 0; i < octaves; i++) 
	{
		output += (amplitude * PelinNoise(x * frequency, y * frequency, pelinFrequancy));
		denom += amplitude;

		frequency *= fbnLacunarity  ;
		amplitude *= fbnPersistence;
	}

	return (output / denom);
}

dimondSquareResult DiamondSquare(int x, int y, float height, float width, float topLeft, float topRight, float bottomLeft, float bottomRight)
{
    dimondSquareResult result;
 
    result.found = false;
    float middleWidth = (width-1) / 2;
    float middleHeight = (height-1) / 2;
	 
 
 	// Get rid of cornors
	if(x == 0 && y ==0)
	{
        result.found = true;
        result.result = topLeft;
     }
	else if (x == (width-2) && y == 0)
	{
        result.found = true;
        result.result = topRight;
     }
	else if (x == 0 && y == (height -2))
	{
        result.found = true;
        result.result = bottomLeft;
     }
	else if (x == (width-2) && y == (height-2))
	{
        result.found = true;
        result.result = bottomRight;
     }

	// middle point of these
	if (x == 0 && y == middleHeight)
	{
        result.found = true;
        result.result = adverage(topLeft, bottomLeft);
     }
	else if (x == middleWidth && y == 0)
	{
        
        result.found = true;
        result.result = adverage(topLeft, topRight);
     }
    else if (x == width && y == middleHeight)
	{
                
        result.found = true;
        result.result = adverage(topRight, bottomRight);
     }
    else if (x == middleWidth && y == height)
	{
        
        result.found = true;
        result.result = adverage(bottomLeft, bottomRight);
     }

	if (x == middleWidth && y == middleHeight)
	{
        
        result.found = true;
        result.result = adverage(bottomLeft, bottomRight, topRight, topLeft);
     }

    // Work out which 4 square pos lays in 
    if(result.found == false)
    {
        if (x > middleWidth)
        {
            if (y > middleHeight)
            {
                result.topLeft = adverage(bottomLeft, bottomRight, topRight, topLeft);
                result.topRight = adverage(topRight, bottomRight);
                result.bottomLeft = adverage(bottomLeft, bottomRight);
                result.bottomRight = bottomRight;

   

            }
            else
            {
                result.topLeft = adverage(topLeft, topRight);
                result.topRight = topRight;
                result.bottomLeft = adverage(bottomLeft, bottomRight, topRight, topLeft);
                result.bottomRight = adverage(topRight, bottomRight);

       
            }
            
        }
        else
        {
            if (y > middleHeight)
            {
                result.topLeft = adverage(topLeft, bottomLeft);
                result.topRight = adverage(bottomLeft, bottomRight, topRight, topLeft);
                result.bottomLeft = bottomLeft;
                result.bottomRight = adverage(bottomLeft, bottomRight);
 

            }
            else
            {
                result.topLeft = topLeft;
                result.topRight = adverage(topLeft, topRight);
                result.bottomLeft = adverage(bottomLeft, topLeft);
                result.bottomRight = adverage(bottomLeft, bottomRight, topRight, topLeft);
  
            }
        }
    }

    return result;

}

float adverage(float a, float b)
{
	return ((a + b) / 2);
}

float adverage(float a, float b ,float c, float d)
{
    return ((a + b + c+ d) / 4);
} 