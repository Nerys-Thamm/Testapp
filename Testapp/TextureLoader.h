// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : TextureLoader.h
// Description : Declares methods for loading textures from files
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

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
	static void CreateFrameBuffer(int _xResolution, int _yResolution, GLuint& out_renderTexture, GLuint& out_frameBuffer);
};

