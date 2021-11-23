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

Noise::Noise()
{
}

Noise& Noise::getInstance()
{
    if (!m_instance)
    {
        m_instance = new Noise();
        srand(std::time(NULL));
    }
    return *m_instance;
    
}


float Noise::RandXY(int _x, int _y)
{
    int n = _x + _y * 57 + (rand() % 10);
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}


float Noise::SmoothNoise(int _x, int _y)
{
    float corners = (RandXY(_x - 1, _y - 1) + RandXY(_x + 1, _y - 1) + RandXY(_x - 1, _y + 1) + RandXY(_x + 1, _y + 1)) / 16;
    float sides = (RandXY(_x - 1, _y) + RandXY(_x + 1, _y) + RandXY(_x, _y - 1) + RandXY(_x, _y + 1)) / 8;
    float center = RandXY(_x, _y) / 4;
    return corners + sides + center;
}


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


float Noise::Lerp(float _a, float _b, float _t)
{
    return _a + _t * (_b - _a);
}


float Noise::Coserp(float _a, float _b, float _t)
{
    float ft = _t * 3.1415927f;
    float f = (1 - cos(ft)) * 0.5f;
    return _a * (1 - f) + _b * f;
}


float Noise::Cuberp(float _v0, float _v1, float _v2, float _v3, float _x)
{
    float P = _v3 - _v2 - _v0 + _v1;
    float Q = _v0 - _v1 - P;
    float R = _v2 - _v0;
    float S = _v1;
    return P * _x * _x * _x + Q * _x * _x + R * _x + S;
}

float Noise::GenerateNoise(int _x, int _y, int octaves, float persistence, float scale)
{
    float total = 0;
    float frequency = scale;
    float amplitude = 1;
    float maxValue = 0;
    for (int i = 0; i < octaves; i++)
    {
        total += Coserp(0, 1, PerlinNoise(_x * frequency, _y * frequency)) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    return total / maxValue;
}

void Noise::PopulateHeightMap(std::vector<unsigned char>& heightMap, int width, int height, int octaves, float persistence, float scale)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            heightMap[x + y * width] = (unsigned char)(GenerateNoise(x, y, octaves, persistence, scale) * 255);
        }
    }
}

Noise* Noise::m_instance = nullptr;

void Noise::SetSeed(float _seed)
{
}
