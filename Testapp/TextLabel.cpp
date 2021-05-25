#include "TextLabel.h"

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

TextLabel::TextLabel(std::string Text, std::string Font, glm::ivec2 PixelSize, glm::vec2 Pos, TextLabel::TextEffect Effect, glm::vec3 Color, glm::vec2 Scale, float LeftBuffer, float RightBuffer)
{
	SetText(Text);
	SetColor(Color);
	SetScale(Scale);
	SetPosition(Pos);

	m_LeftBuffer = LeftBuffer;
	m_RightBuffer = RightBuffer;

	ProjectionMat = glm::ortho(00.0f, (GLfloat)cfWINDOW_WIDTH(), 0.0f, (GLfloat)cfWINDOW_HEIGHT(), 0.0f, 100.0f);
	switch (Effect)
	{
	case TextLabel::NONE:
		Program_Text = ShaderLoader::CreateProgram("Resources/Shaders/Text.vs", "Resources/Shaders/Text.fs");
		break;
	case TextLabel::MARQUEE:
		Program_Text = ShaderLoader::CreateProgram("Resources/Shaders/TextMarquee.vs", "Resources/Shaders/TextMarquee.fs");
		break;
	case TextLabel::SCALE_BOUNCE:
		Program_Text = ShaderLoader::CreateProgram("Resources/Shaders/TextScaleBounce.vs", "Resources/Shaders/TextScaleBounce.fs");
		break;
	default:
		break;
	}
	

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
	glUniform1f(CurrentTimeLoc, glfwGetTime());
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
