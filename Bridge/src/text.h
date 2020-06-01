// MY TEXT Header TODO : CHECK IT maybe cause text render bug
#ifndef STB_TRUETYPE_IMPLEMENTATION

#define STB_TRUETYPE_IMPLEMENTATION

struct stbtt_char_t
{
	GLuint	textureID;				// ID handle of the glyph texture
	ivec2	size;					// Size of glyph
	ivec2	bearing;				// Offset from baseline to left/top of glyph
	GLuint	advance;				// Horizontal offset to advance to next glyph
};

void create_font_textures();
bool init_text();
void render_text(std::string text, GLint _x, GLint _y, GLfloat scale, vec4 color, GLfloat dpi_scale);

#endif