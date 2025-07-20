#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glut.h>
#include <glm.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <gl/glext.h>

struct Character {
    GLuint texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};

class Font {
public:
    Font(const std::string& font_path, int font_size) {
        if (FT_Init_FreeType(&ft_library)) {
            std::cerr << "Could not init FreeType Library\n";
            exit(EXIT_FAILURE);
        }
        if (FT_New_Face(ft_library, font_path.c_str(), 0, &ft_face)) {
            std::cerr << "Failed to load font: " << font_path << "\n";
            exit(EXIT_FAILURE);
        }
        FT_Set_Pixel_Sizes(ft_face, 0, font_size);
        load_characters(font_size);
    }
    ~Font() {
        FT_Done_Face(ft_face);
        FT_Done_FreeType(ft_library);
    }

    const std::unordered_map<GLchar, Character>& get_characters() const {
        return characters;
    }

private:
    FT_Library ft_library;
    FT_Face ft_face;
    std::unordered_map<GLchar, Character> characters;

    void load_characters(int font_size) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        for (GLubyte c = 0; c < 128; ++c) {
            if (FT_Load_Char(ft_face, c, FT_LOAD_RENDER)) {
                std::cerr << "Failed to load Glyph " << c << "\n";
                continue;
            }
            GLuint tex;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_ALPHA,
                ft_face->glyph->bitmap.width,
                ft_face->glyph->bitmap.rows,
                0,
                GL_ALPHA,
                GL_UNSIGNED_BYTE,
                ft_face->glyph->bitmap.buffer
            );

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            Character character = {
                tex,
                { ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows },
                { ft_face->glyph->bitmap_left, ft_face->glyph->bitmap_top },
                static_cast<GLuint>(ft_face->glyph->advance.x)
            };
            characters.emplace(c, character);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }
};
