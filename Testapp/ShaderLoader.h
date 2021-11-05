// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : ShaderLoader.h
// Description : Header file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#pragma once

// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include <cstdarg>

class ShaderLoader
{
public:
	struct ShaderFile
	{
		const char* Filename;
		GLenum Type;
	};
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename, const char* GeometryShaderFilename);
	static GLuint CreateProgram(const std::initializer_list<ShaderFile> _shaders);

private:
	ShaderLoader(void);
	~ShaderLoader(void);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char* filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};
