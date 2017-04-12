// colour vertex shader
// Simple geometry pass
// texture coordinates and normals will be ignored.

cbuffer MatrixBuffer : register(cb0)
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
	float3 normal : NORMAL;
};
 


int fastfloor(double x)
{
    int xi = (int) x;
    return x < xi ? xi - 1 : xi;
}

double dot(float3 g, double x, double y)
{
    return g.x * x + g.y * y;
}

double dot(float3 g, double x, double y, double z)
{
    return g.x * x + g.y * y + g.z * z;
}
float grad(int hash, float x)
{
    uint h = hash & 0x0F; // Convert low 4 bits of hash code
    float grad = 1.0f + (h & 7); // Gradient value 1.0, 2.0, ..., 8.0
    if ((h & 8) != 0)
        grad = -grad; // Set a random sign for the gradient
									//  float grad = gradients1D[h];    // NOTE : Test of Gradient look-up table instead of the above
    return (grad * x); // Multiply the gradient with the distance
}
float grad(int hash, float x, float y)
{
    uint h = hash & 0x3F; // Convert low 3 bits of hash code
    float u = h < 4 ? x : y; // into 8 simple gradient directions,
    float v = h < 4 ? y : x; // and compute the dot product with (x,y).
    return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
}

float noise(float x)
{
    uint perm[256] =
    {
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
 
    float n0, n1; // Noise contributions from the two "corners"

					// No need to skew the input space in 1D

					// Corners coordinates (nearest integer values):
    int i0 = fastfloor(x);
    int i1 = i0 + 1;
	// Distances to corners (between 0 and 1):
    float x0 = x - i0;
    float x1 = x0 - 1.0f;

	// Calculate the contribution from the first corner
    float t0 = 1.0f - x0 * x0;
	//  if(t0 < 0.0f) t0 = 0.0f; // not possible
    t0 *= t0;
    n0 = t0 * t0 * grad(perm[i0], x0);

	// Calculate the contribution from the second corner
    float t1 = 1.0f - x1 * x1;
	//  if(t1 < 0.0f) t1 = 0.0f; // not possible
    t1 *= t1;
    n1 = t1 * t1 * grad(perm[i1], x1);

	// The maximum value of this noise is 8*(3/4)^4 = 2.53125
	// A factor of 0.395 scales to fit exactly within [-1,1]
    return 0.395f * (n0 + n1);
}

 
float noise(float x, float y)
{
    int perm[256] =
    {
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
     
    float n0, n1, n2; // Noise contributions from the three corners

						// Skewing/Unskewing factors for 2D
    float F2 = 0.366025403f; // F2 = (sqrt(3) - 1) / 2
    float G2 = 0.211324865f; // G2 = (3 - sqrt(3)) / 6   = F2 / (1 + 2 * K)

									// Skew the input space to determine which simplex cell we're in
    float s = (x + y) * F2; // Hairy factor for 2D
    float xs = x + s;
    float ys = y + s;
    uint i = fastfloor(xs);
    uint j = fastfloor(ys);

	// Unskew the cell origin back to (x,y) space
    float t = ( float)  (i + j) * G2;
    float X0 = i - t;
    float Y0 = j - t;
    float x0 = x - X0; // The x,y distances from the cell origin
    float y0 = y - Y0;

	// For the 2D case, the simplex shape is an equilateral triangle.
	// Determine which simplex we are in.
    uint i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
    if (x0 > y0)
    { // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        i1 = 1;
        j1 = 0;
    }
    else
    { // upper triangle, YX order: (0,0)->(0,1)->(1,1)
        i1 = 0;
        j1 = 1;
    }

	// A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
	// a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
	// c = (3-sqrt(3))/6

    float x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    float y1 = y0 - j1 + G2;
    float x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
    float y2 = y0 - 1.0f + 2.0f * G2;

	// Calculate the contribution from the first corner
    float t0 = 0.5f - x0 * x0 - y0 * y0;
    if (t0 < 0.0f)
    {
        n0 = 0.0f;
    }
    else
    {
        t0 *= t0;
        n0 = t0 * t0 * grad(perm[i + perm[j]], x0, y0);
    }

	// Calculate the contribution from the second corner
    float t1 = 0.5f - x1 * x1 - y1 * y1;
    if (t1 < 0.0f)
    {
        n1 = 0.0f;
    }
    else
    {
        t1 *= t1;
        n1 = t1 * t1 * grad(perm[i + i1 + perm[(j + j1)]],
        x1, y1);
    }

	// Calculate the contribution from the third corner
    float t2 = 0.5f - x2 * x2 - y2 * y2;
    if (t2 < 0.0f)
    {
        n2 = 0.0f;
    }
    else
    {
        t2 *= t2;
        n2 = t2 * t2 * grad(perm[i + 1 + perm[j + 1]],
        x2, y2);
    }

	// Add contributions from each corner to get the final noise value.
	// The result is scaled to return values in the interval [-1,1].
    return 45.23065f * (n0 + n1 + n2);
}


float fbm(float x,float y,int octave)
{
  
   
    float output = 0.f;
    float denom = 0.f;
    float frequency = 8.0f;
    float amplitude = 1.0f;

    for (int i = 0; i < octave; i++)
    {
        output += (amplitude * noise((double) x * (double) frequency, (double) y * (double) frequency));
        denom += amplitude;

        frequency *= .5;
        amplitude *= .5;
    }

    return (output / denom);
}

OutputType main(InputType input)
{
	OutputType output;




	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;
 


    //input.position.y = noise(input.position.x * 0.99f, input.position.y * 0.99f);
 
	
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
