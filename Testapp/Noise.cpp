// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : Noise.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "Noise.h"

/// <summary>
/// Initializes a new instance of the Noise class.
/// </summary>
Noise::Noise()
{
}

/// <summary>
/// Gets an instance of the Noise class.
/// </summary>
Noise& Noise::getInstance()
{
    if (!m_instance)
    {
        m_instance = new Noise();
    }
    return *m_instance;
    
}


// ********************************************************************************
/// <summary>
///  Gets the noise value.
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <returns></returns>
// ********************************************************************************
float Noise::RandXY(int _x, int _y)
{
    

    int n = _x + _y * m_seed; //Generates a position based seed
    n = (n << 13) ^ n; //Adds a bitwise shift to the seed
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f); //Returns a float value between 0 and 1
}


// ********************************************************************************
/// <summary>
///  Gets the smooth noise value.
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <returns></returns>
// ********************************************************************************
float Noise::SmoothNoise(int _x, int _y)
{
    float corners = (RandXY(_x - 1, _y - 1) + RandXY(_x + 1, _y - 1) + RandXY(_x - 1, _y + 1) + RandXY(_x + 1, _y + 1)) / 16; 
    float sides = (RandXY(_x - 1, _y) + RandXY(_x + 1, _y) + RandXY(_x, _y - 1) + RandXY(_x, _y + 1)) / 8; 
    float center = RandXY(_x, _y) / 4;
    return corners + sides + center;
}


// ********************************************************************************
/// <summary>
///  Gets the interpolated noise value.
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <returns></returns>
// ********************************************************************************
float Noise::PerlinNoise(float _x, float _y)
{
    float fractional_X = _x - (int)_x; 
    float fractional_Y = _y - (int)_y; 

    float v1 = SmoothNoise((int)_x, (int)_y); 
    float v2 = SmoothNoise((int)_x + 1, (int)_y);
    float v3 = SmoothNoise((int)_x, (int)_y + 1);
    float v4 = SmoothNoise((int)_x + 1, (int)_y + 1);

    float i1 = Coserp(v1, v2, fractional_X);
    float i2 = Coserp(v3, v4, fractional_X);

    return Coserp(i1, i2, fractional_Y);
}


// ********************************************************************************
/// <summary>
///  LERP between two values
/// </summary>
/// <param name="_a"></param>
/// <param name="_b"></param>
/// <param name="_t"></param>
/// <returns></returns>
// ********************************************************************************
float Noise::Lerp(float _a, float _b, float _t)
{
    return _a + _t * (_b - _a);
}


// ********************************************************************************
/// <summary>
///  Cosine interpolation between two values
/// </summary>
/// <param name="_a"></param>
/// <param name="_b"></param>
/// <param name="_t"></param>
/// <returns></returns>
// ********************************************************************************
float Noise::Coserp(float _a, float _b, float _t)
{
    float ft = _t * 3.1415927f;
    float f = (1 - cos(ft)) * 0.5f;
    return _a * (1 - f) + _b * f;
}


// ********************************************************************************
/// <summary>
///  Cubic interpolation between two values
/// </summary>
/// <param name="_v0"></param>
/// <param name="_v1"></param>
/// <param name="_v2"></param>
/// <param name="_v3"></param>
/// <param name="_x"></param>
/// <returns></returns>
// ********************************************************************************
float Noise::Cuberp(float _v0, float _v1, float _v2, float _v3, float _x)
{
    float P = _v3 - _v2 - _v0 + _v1;
    float Q = _v0 - _v1 - P;
    float R = _v2 - _v0;
    float S = _v1;
    return P * _x * _x * _x + Q * _x * _x + R * _x + S;
}

// ********************************************************************************
/// <summary>
///  Gets the perlin noise value.
/// </summary>
/// <param name="_x"></param>
/// <param name="_y"></param>
/// <param name="octaves"></param>
/// <param name="persistence"></param>
/// <param name="scale"></param>
/// <returns></returns>
// ********************************************************************************
float Noise::GenerateNoise(int _x, int _y, int octaves, float persistence, float scale)
{
    float total = 0;
    float frequency = scale;
    float amplitude = 1;
    float maxValue = 0;
    for (int i = 0; i < octaves; i++) // For each octave
    {
        total += Coserp(0, 1, PerlinNoise(_x * frequency, _y * frequency)) * amplitude; // Add weighted noise value to final total
        maxValue += amplitude; // Keep track of amplitude, for normalisation
        amplitude *= persistence; // Update amplitude, for next octave
        frequency *= 2; // Update frequency, for next octave
    }
    return total / maxValue;
}

// ********************************************************************************
/// <summary>
///  Populates a heightmap with perlin noise.
/// </summary>
/// <param name="heightMap"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="octaves"></param>
/// <param name="persistence"></param>
/// <param name="scale"></param>
// ********************************************************************************
void Noise::PopulateHeightMap(std::vector<unsigned char>& heightMap, int width, int height, int octaves, float persistence, float scale)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            heightMap[x + y * width] = (unsigned char)(GenerateNoise(x, y, octaves, persistence, scale) * 150); // Generate noise value, convert to 0-255 and store in array
        }
    }
}

Noise* Noise::m_instance = nullptr;

// ********************************************************************************
/// <summary>
///  Sets the random seed
/// </summary>
/// <param name="_seed"></param>
// ********************************************************************************
void Noise::SetSeed(float _seed)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(30, 100); // define the range
    m_seed = distr(gen); // generate number
}
