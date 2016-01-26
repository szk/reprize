#ifndef IMPROVED_NOISE_HPP_
#define IMPROVED_NOISE_HPP_

// JAVA REFERENCE IMPLEMENTATION OF IMPROVED NOISE - COPYRIGHT 2002 KEN PERLIN.

class ImprovedNoise
{
public:
    ImprovedNoise(void)
        {
            p = new int[512];
            int permutation[] =
                { 151,160,137,91,90,15,
                  131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
                  190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
                  88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
                  77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
                  102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
                  135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
                  5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
                  223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
                  129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
                  251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
                  49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                  138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };
            for (int i=0; i < 256 ; i++) { p[256+i] = p[i] = permutation[i]; }
        }
    virtual ~ImprovedNoise(void) {}
    double noise(double x, double y, double z)
        {
            int X = (int)floor(x) & 255, // FIND UNIT CUBE THAT
                Y = (int)floor(y) & 255, // CONTAINS POINT.
                Z = (int)floor(z) & 255;
            x -= floor(x); // FIND RELATIVE X,Y,Z
            y -= floor(y); // OF POINT IN CUBE.
            z -= floor(z);
            double u = fade(x), // COMPUTE FADE CURVES
                v = fade(y),    // FOR EACH OF X,Y,Z.
                w = fade(z);
            int A = p[X  ]+Y, AA = p[A]+Z, AB = p[A+1]+Z, // HASH COORDINATES OF
                B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z; // THE 8 CUBE CORNERS,

            return lerp(w, lerp(v,
                                lerp(u,
                                     grad(p[AA  ], x  , y  , z   ), // AND ADD
                                     grad(p[BA  ], x-1, y  , z   )), // BLENDED
                                lerp(u,
                                     grad(p[AB  ], x  , y-1, z   ),  // RESULTS
                                     grad(p[BB  ], x-1, y-1, z   ))),// FROM  8
                        lerp(v, lerp(u,
                                     grad(p[AA+1], x  , y  , z-1 ),  // CORNERS
                                     grad(p[BA+1], x-1, y  , z-1 )), // OF CUBE
                             lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
                                  grad(p[BB+1], x-1, y-1, z-1 ))));
        }
private:
    inline double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    inline double lerp(double t, double a, double b) { return a + t * (b - a); }
    inline double grad(int hash, double x, double y, double z)
        {
            int h = hash & 15;      // CONVERT LO 4 BITS OF HASH CODE
            double u = h<8 ? x : y, // INTO 12 GRADIENT DIRECTIONS.
                v = h<4 ? y : h==12||h==14 ? x : z;
            return ((h&1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
        }
    int* p;
};

#endif

#ifdef UNDEF
// Other preparetion: Worley noise, http://aftbit.com/cell-noise-2/

class CellNoise
{
    /// <summary>
    /// Generates Cell Noise
    /// </summary>
    /// <param name="input">The location at which the cell noise function should be evaluated at.</param>
    /// <param name="seed">The seed for the noise function</param>
    /// <param name="distanceFunc">The function used to calculate the distance between two points. Several of these are defined as statics on the WorleyNoise class.</param>
    /// <param name="distanceArray">An array which will store the distances computed by the Worley noise function.
    /// The length of the array determines how many distances will be computed.</param>
    /// <param name="combineDistancesFunc">The function used to color the location. The color takes the populated distanceArray
    /// param and returns a float which is the greyscale value outputed by the worley noise function.</param>
    /// <returns>The color worley noise returns at the input position</returns>
    public static Vector4 CellNoiseFunc(this Vector3 input, int seed, Func<Vector3, Vector3, float> distanceFunc, ref float[] distanceArray, Func<float[], float> combineDistancesFunc)
    {
        //Declare some values for later use
        uint lastRandom, numberFeaturePoints;
        Vector3 randomDiff, featurePoint;
        int cubeX, cubeY, cubeZ;
 
        //Initialize values in distance array to large values
        for (int i = 0; i < distanceArray.Length; i++)
            distanceArray[i] = 6666;
 
        //1. Determine which cube the evaluation point is in
        int evalCubeX = (int)Math.Floor(input.X);
        int evalCubeY = (int)Math.Floor(input.Y);
        int evalCubeZ = (int)Math.Floor(input.Z);
 
        for (int i = -1; i < 2; ++i)
            for (int j = -1; j < 2; ++j)
                for (int k = -1; k < 2; ++k)
                {
                    cubeX = evalCubeX + i;
                    cubeY = evalCubeY + j;
                    cubeZ = evalCubeZ + k;
 
                    //2. Generate a reproducible random number generator for the cube
                    lastRandom = lcgRandom(hash((uint)(cubeX + seed), (uint)(cubeY), (uint)(cubeZ)));
                    //3. Determine how many feature points are in the cube
                    numberFeaturePoints = probLookup(lastRandom);
                    //4. Randomly place the feature points in the cube
                    for (uint l = 0; l < numberFeaturePoints; ++l)
                    {
                        lastRandom = lcgRandom(lastRandom);
                        randomDiff.X = (float)lastRandom / 0x100000000;
 
                        lastRandom = lcgRandom(lastRandom);
                        randomDiff.Y = (float)lastRandom / 0x100000000;
 
                        lastRandom = lcgRandom(lastRandom);
                        randomDiff.Z = (float)lastRandom / 0x100000000;
 
                        featurePoint = new Vector3(randomDiff.X + (float)cubeX, randomDiff.Y + (float)cubeY, randomDiff.Z + (float)cubeZ);
 
                        //5. Find the feature point closest to the evaluation point.
                        //This is done by inserting the distances to the feature points into a sorted list
                        insert(distanceArray, distanceFunc(input, featurePoint));
                    }
                    //6. Check the neighboring cubes to ensure their are no closer evaluation points.
                    // This is done by repeating steps 1 through 5 above for each neighboring cube
                }
 
        float color = combineDistancesFunc(distanceArray);
        if(color < 0) color = 0;
        if(color > 1) color = 1;
        return new Vector4(color, color, color, 1);
    }
 
    public static float EuclidianDistanceFunc(Vector3 p1, Vector3 p2)
    {
        return (p1.X - p2.X) * (p1.X - p2.X) + (p1.Y - p2.Y) * (p1.Y - p2.Y) + (p1.Z - p2.Z) * (p1.Z - p2.Z);
    }
 
    public static float ManhattanDistanceFunc(Vector3 p1, Vector3 p2)
    {
        return Math.Abs(p1.X - p2.X) + Math.Abs(p1.Y - p2.Y) + Math.Abs(p1.Z - p2.Z);
    }
 
    public static float ChebyshevDistanceFunc(Vector3 p1, Vector3 p2)
    {
        Vector3 diff = p1 - p2;
        return Math.Max(Math.Max(Math.Abs(diff.X), Math.Abs(diff.Y)), Math.Abs(diff.Z));
    }
 
    /// <summary>
    /// Given a uniformly distributed random number this function returns the number of feature points in a given cube.
    /// </summary>
    /// <param name="value">a uniformly distributed random number</param>
    /// <returns>The number of feature points in a cube.</returns>
    // Generated using mathmatica with "AccountingForm[N[Table[CDF[PoissonDistribution[4], i], {i, 1, 9}], 20]*2^32]"
    private static uint probLookup(uint value)
    {
        if (value < 393325350) return 1;
        if (value < 1022645910) return 2;
        if (value < 1861739990) return 3;
        if (value < 2700834071) return 4;
        if (value < 3372109335) return 5;
        if (value < 3819626178) return 6;
        if (value < 4075350088) return 7;
        if (value < 4203212043) return 8;
        return 9;
    }
 
    /// <summary>
    /// Inserts value into array using insertion sort. If the value is greater than the largest value in the array
    /// it will not be added to the array.
    /// </summary>
    /// <param name="arr">The array to insert the value into.</param>
    /// <param name="value">The value to insert into the array.</param>
    private static void insert(float[] arr, float value)
    {
        float temp;
        for (int i = arr.Length - 1; i >= 0; i--)
        {
            if (value > arr[i]) break;
            temp = arr[i];
            arr[i] = value;
            if (i + 1 < arr.Length) arr[i + 1] = temp;
        }
    }
 
    /// <summary>
    /// LCG Random Number Generator.
    /// LCG: http://en.wikipedia.org/wiki/Linear_congruential_generator
    /// </summary>
    /// <param name="lastValue">The last value calculated by the lcg or a seed</param>
    /// <returns>A new random number</returns>
    private static uint lcgRandom(uint lastValue)
    {
        return (uint)((1103515245u * lastValue + 12345u) % 0x100000000u);
    }
 
    /// <summary>
    /// Constant used in FNV hash function.
    /// FNV hash: http://isthe.com/chongo/tech/comp/fnv/#FNV-source
    /// </summary>
    private const uint OFFSET_BASIS = 2166136261;
    /// <summary>
    /// Constant used in FNV hash function
    /// FNV hash: http://isthe.com/chongo/tech/comp/fnv/#FNV-source
    /// </summary>
    private const uint FNV_PRIME = 16777619;
    /// <summary>
    /// Hashes three integers into a single integer using FNV hash.
    /// FNV hash: http://isthe.com/chongo/tech/comp/fnv/#FNV-source
    /// </summary>
    /// <returns>hash value</returns>
    private static uint hash(uint i, uint j, uint k)
    {
        return (uint)((((((OFFSET_BASIS ^ (uint)i) * FNV_PRIME) ^ (uint)j) * FNV_PRIME) ^ (uint)k) * FNV_PRIME);
    }
}

#endif
