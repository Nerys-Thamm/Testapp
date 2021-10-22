// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name   : TextureLoader.cpp
// Description : Implements methods for loading textures from files
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz

#include "TextureLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// ********************************************************************************
/// <summary>
/// Loads a 2D texture
/// </summary>
/// <param name="_filename"></param>
/// <returns></returns>
// ********************************************************************************
GLuint TextureLoader::LoadTexture(std::string _filename)
{
	
		stbi_set_flip_vertically_on_load(true);

		GLuint out;

		// Load the Image data
		int ImageWidth;
		int ImageHeight;
		int ImageComponents;
		unsigned char* ImageData = stbi_load(("Resources/Textures/" + _filename).c_str(), &ImageWidth, &ImageHeight, &ImageComponents, 0);

		

		//Create and bind a new texture template
		glGenTextures(1, &out);
		glBindTexture(GL_TEXTURE_2D, out);

		// Check how many components the loaded image has (rgb or rgba)
		GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

		// Populate the texture with image data
		glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

		// Generate mipmaps, free memory and unbind texture
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(ImageData);
		glBindTexture(GL_TEXTURE_2D, 0);
		return out;
	
}

// ********************************************************************************
/// <summary>
/// Loads a Cubemap texture
/// </summary>
/// <param name="_filepaths"></param>
/// <returns></returns>
// ********************************************************************************
GLuint TextureLoader::LoadCubeMap(std::string _filepaths[6])
{
	stbi_set_flip_vertically_on_load(false);

	GLuint out;

	//Load cubemap
	glGenTextures(1, &out);
	glBindTexture(GL_TEXTURE_CUBE_MAP, out);

	// Load the Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	//GLbyte Faces[] = { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y };
	
	//Setting the address mode for the texture
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	for (size_t i = 0; i < 6; i++)
	{
		unsigned char* ImageData = stbi_load(("Resources/Textures/Cubemaps/" + _filepaths[i]).c_str(), &ImageWidth, &ImageHeight, &ImageComponents, 0);



		// Check how many components the loaded image has (rgb or rgba)
		GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

		// Populate the texture with image data
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);



		stbi_image_free(ImageData);
	}


	// Generate mipmaps, free memory and unbind texture
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return out;
}

void TextureLoader::CreateFrameBuffer(int _xResolution, int _yResolution, GLuint& out_renderTexture, GLuint& out_frameBuffer)
{
	
	glGenTextures(1, &out_renderTexture);
	glBindTexture(GL_TEXTURE_2D, out_renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _xResolution, _yResolution, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	
	glGenFramebuffers(1, &out_frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, out_frameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, out_renderTexture, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _xResolution, _yResolution);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) std::cout << "ERROR::FRAMEBUFFER::Framebuffer is not complete!" << std::endl;
	
}
