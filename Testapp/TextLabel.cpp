// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
// 
// (c) 2021 Media Design School
//
// File Name   : TextLabel.cpp
// Description : Implementation file
// Author      : Nerys Thamm
// Mail        : nerys.thamm@mds.ac.nz
#include "TextLabel.h"

// ********************************************************************************
/// <summary>
/// Generates a texture for a character
/// </summary>
/// <param name="face"></param>
/// <returns></returns>
// ********************************************************************************
GLuint TextLabel::GenerateTexture(FT_Face face)
{
	GLuint TextureID;
	glGenTextures(1, &TextureID);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return TextureID;
}

// ********************************************************************************
/// <summary>
/// Is called every frame
/// </summary>
/// <param name="_fDeltaTime"></param>
// ********************************************************************************
void TextLabel::Update(float _fDeltaTime)
{
	if (m_scaleBouncing)
	{
		float textWidth = 0.0f;
		float textHeight = 0.0f;
		for (std::string::const_iterator TextCharacter = TextStr.begin(); TextCharacter != TextStr.end(); TextCharacter++)
		{
			FontChar FontCharacter = CharacterMap[*TextCharacter];
			textWidth += FontCharacter.Size.x * Scale.x;
			textHeight = FontCharacter.Size.y * Scale.y;
		}
		glm::vec2 centre = glm::vec2(textWidth / 2, textHeight / 2);
		SetScale(glm::vec2(((sin(glfwGetTime() * 3) + 1) / 4) + 1, ((sin(glfwGetTime() * 3) + 1) / 4) + 1));
		textWidth = 0.0f;
		textHeight = 0.0f;
		for (std::string::const_iterator TextCharacter = TextStr.begin(); TextCharacter != TextStr.end(); TextCharacter++)
		{
			FontChar FontCharacter = CharacterMap[*TextCharacter];
			textWidth += FontCharacter.Size.x * Scale.x;
			textHeight = FontCharacter.Size.y * Scale.y;
		}
		glm::vec2 newcentre = glm::vec2(textWidth / 2, textHeight / 2);
		SetPosition(Position - (newcentre - centre));
	}
}

// ********************************************************************************
/// <summary>
/// Constructor
/// </summary>
/// <param name="Text">The content of the TextLabel</param>
/// <param name="Font">The font file</param>
/// <param name="PixelSize">The size in pixels of the characters</param>
/// <param name="Pos">The screen position</param>
/// <param name="Effect">A shader effect for the label</param>
/// <param name="Color">The color of the label</param>
/// <param name="Scale">The scale of the label</param>
/// <param name="LeftBuffer">Label will not render left of this point</param>
/// <param name="RightBuffer">Label will not render right of this point</param>
/// <returns></returns>
// ********************************************************************************
TextLabel::TextLabel(std::string Text, std::string Font, glm::ivec2 PixelSize, glm::vec2 Pos, TextLabel::TextEffect Effect, glm::vec3 Color, glm::vec2 Scale, float LeftBuffer, float RightBuffer)
{
	SetText(Text);
	SetColor(Color);
	SetScale(Scale);
	SetPosition(Pos);

	m_LeftBuffer = LeftBuffer;
	m_RightBuffer = RightBuffer;

	ProjectionMat = glm::ortho(00.0f, (GLfloat)cfWINDOW_WIDTH(), 0.0f, (GLfloat)cfWINDOW_HEIGHT(), 0.0f, 100.0f);

	//Set the effect
	switch (Effect)
	{
	case TextLabel::TextEffect::NONE:
		Program_Text = ShaderLoader::CreateProgram("Resources/Shaders/Text.vert", "Resources/Shaders/Text.frag");
		break;
	case TextLabel::TextEffect::MARQUEE:
		Program_Text = ShaderLoader::CreateProgram("Resources/Shaders/TextMarquee.vert", "Resources/Shaders/TextMarquee.frag");
		break;
	case TextLabel::TextEffect::SCALE_BOUNCE:
		Program_Text = ShaderLoader::CreateProgram("Resources/Shaders/Text.vert", "Resources/Shaders/Text.frag");
		m_scaleBouncing = true;
		break;
	default:
		break;
	}


	//Generate the Text
	FT_Library FontLibrary;
	FT_Face FontFace;

	if (FT_Init_FreeType(&FontLibrary) != 0)
	{
		std::cout << "FreeType Error: Could not init FreeType Library" << std::endl;
		return;
	}

	if (FT_New_Face(FontLibrary, Font.c_str(), 0, &FontFace) != 0)
	{
		std::cout << "FreeType Error: Failed to load font:" << Font << std::endl;
		return;
	}

	FT_Set_Pixel_Sizes(FontFace, PixelSize.x, PixelSize.y);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (GLubyte Glyph = 0; Glyph < FontCharacterLimit; Glyph++)
	{
		if (FT_Load_Char(FontFace, Glyph, FT_LOAD_RENDER))
		{
			std::cout << "FreeType Error: Failed to load Glyph: " << (unsigned char)Glyph << std::endl;
			continue;
		}

		FontChar FontCharacter = {
			GenerateTexture(FontFace),
			glm::ivec2(FontFace->glyph->bitmap.width, FontFace->glyph->bitmap.rows),
			glm::ivec2(FontFace->glyph->bitmap_left, FontFace->glyph->bitmap_top),
			(GLuint)FontFace->glyph->advance.x / 64
		};
		CharacterMap.insert(std::pair<GLchar, FontChar>(Glyph, FontCharacter));
	}

	FT_Done_Face(FontFace);
	FT_Done_FreeType(FontLibrary);

	glGenVertexArrays(1, &VAO_Text);
	glBindVertexArray(VAO_Text);

	//Generate and bind EBO
	glGenBuffers(1, &EBO_Text);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Text);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO_DynamicQuad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_DynamicQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * 4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	Initialized = true;
}

// ********************************************************************************
/// <summary>
/// Renders the TextLabel
/// </summary>
// ********************************************************************************
void TextLabel::Render()
{
	if (!Initialized)
	{
		return;
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glUseProgram(Program_Text);
	glUniform3fv(glGetUniformLocation(Program_Text, "TextColor"), 1, glm::value_ptr(Color));
	glUniformMatrix4fv(glGetUniformLocation(Program_Text, "ProjectionMat"), 1, GL_FALSE, glm::value_ptr(ProjectionMat));
	GLint CurrentTimeLoc = glGetUniformLocation(Program_Text, "CurrentTime");
	glUniform1f(CurrentTimeLoc, (GLfloat)glfwGetTime());
	GLint ScreenWidthLoc = glGetUniformLocation(Program_Text, "ScreenWidth");
	glUniform1f(ScreenWidthLoc, cfWINDOW_WIDTH());
	GLint LeftBufferLoc = glGetUniformLocation(Program_Text, "LeftBuffer");
	glUniform1f(LeftBufferLoc, m_LeftBuffer);
	GLint RightBufferLoc = glGetUniformLocation(Program_Text, "RightBuffer");
	glUniform1f(RightBufferLoc, m_RightBuffer);
	glBindVertexArray(VAO_Text);

	glm::vec2 CharacterOrigin = Position;

	for (std::string::const_iterator TextCharacter = TextStr.begin(); TextCharacter != TextStr.end(); TextCharacter++)
	{
		FontChar FontCharacter = CharacterMap[*TextCharacter];
		GLfloat PosX = CharacterOrigin.x + FontCharacter.Bearing.x * Scale.x;
		GLfloat PosY = CharacterOrigin.y - (FontCharacter.Size.y - FontCharacter.Bearing.y) * Scale.y;
		GLfloat Width = FontCharacter.Size.x * Scale.x;
		GLfloat Height = FontCharacter.Size.y * Scale.y;

		GLfloat vertices[4][4] = {
			{PosX, PosY + Height, 0.0, 0.0}, {PosX, PosY, 0.0, 1.0},
			{PosX + Width, PosY, 1.0, 1.0}, {PosX + Width, PosY + Height, 1.0, 0.0},
		};

		glBindBuffer(GL_ARRAY_BUFFER, VBO_DynamicQuad);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, FontCharacter.TextureID);
		glUniform1i(glGetUniformLocation(Program_Text, "TextTexture"), 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		CharacterOrigin.x += FontCharacter.Advance * Scale.x;
	}

	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_BLEND);
}