#pragma once
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <iostream>


class TextureLoader
{
public:
	static GLuint LoadTexture(std::string _filename);
	static GLuint LoadCubeMap(std::string _filepaths[6]);
};

