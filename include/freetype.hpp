#include <ft2build.h>
#include "freetype/freetype.h"
#include FT_FREETYPE_H  

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "GL/glew.h"
#include <iostream>
#include <map>


struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};



class FreeType {
public:
	FreeType();
	void loadFont();
private:

	std::map<GLchar, Character> Characters;
	GLuint VAO, VBO;
	glm::mat4 projection;
};