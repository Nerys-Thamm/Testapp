#include <iostream>
#include <vector>
#include <cmath>

//Noise singleton class
class Noise
{
public:
    //Get the instance of the noise class
    static Noise& getInstance();

    float GenerateNoise(int _x, int _y, int octaves, float persistence, float scale);
    void PopulateHeightMap(std::vector<unsigned char>& heightMap, int width, int height, int octaves, float persistence, float scale);

    float RandXY(int _x, int _y);
    float SmoothNoise(int _x, int _y);
    float PerlinNoise(float _x, float _y);
    float Lerp(float _a, float _b, float _t);
    float Coserp(float _a, float _b, float _t);
    float Cuberp(float _v0, float _v1, float _v2, float _v3, float _x);

    void SetSeed(float _seed);

private:
    //Constructor
    Noise();
    static Noise* m_instance;
    float m_seed;

};
