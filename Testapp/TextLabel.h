#pragma once
// Dependency Includes
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

// Library Includes
#include <map>
#include <string>
#include <iostream>

// Local Includes
#include "ShaderLoader.h"
#include "Config.h"
#include "GameObject.h"

class TextLabel :
	public CGameObject
{
public:
	enum class TextEffect
	{
		NONE,
		MARQUEE,
		SCALE_BOUNCE,
	};
private:
	struct FontChar
	{
		GLuint TextureID = 0; // Texture ID
		glm::ivec2 Size;      // Size of the glyph
		glm::ivec2 Bearing;   // Offset of the glyph (top left) from the baseline
		GLuint Advance = 0;   // How far to move for the next character
	};

	static const int FontCharacterLimit = 128;
	bool Initialized = false;

	bool m_scaleBouncing = false;

	std::string TextStr;
	glm::vec2 Scale;
	glm::vec3 Color;
	glm::vec2 Position;

	float m_LeftBuffer;
	float m_RightBuffer;

	GLuint VAO_Text;
	GLuint VBO_DynamicQuad;
	GLuint EBO_Text;
	GLuint Program_Text;
	glm::mat4 ProjectionMat;
	std::map<GLchar, FontChar> CharacterMap;

	GLuint indices[6] = {
		0,1,2,
		0,2,3,
	};

public:

	TextLabel(
		std::string Text,
		std::string Font,
		glm::ivec2 PixelSize,
		glm::vec2 Pos,
		TextEffect Effect,
		glm::vec3 Color = glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec2 Scale = glm::vec2(1.0f, 1.0f),
		float LeftBuffer = 0,
		float RightBuffer = cfWINDOW_WIDTH()
	);

	~TextLabel() {};

	void Render();
	void SetText(std::string Text) { this->TextStr = Text; };
	void SetColor(glm::vec3 Color) { this->Color = Color; };
	void SetScale(glm::vec2 Scale) { this->Scale = Scale; };
	void SetPosition(glm::vec2 Position) { this->Position = Position; };
private:
	GLuint GenerateTexture(FT_Face face);
	virtual void Update(float _fDeltaTime);
};
