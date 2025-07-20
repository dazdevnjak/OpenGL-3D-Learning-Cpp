#pragma once

#include "Component.h"
#include "Font.h"
#include <glm.hpp>
#include <string>

class Text : public Component {
public:
    Text(Font* f, const std::string& c, glm::vec3 col = glm::vec3(1.0f))
        : font(f), text(c), color(col) {
    }

    void render() override {
        const auto& chars = font->get_characters();
        float x = 0.0f;

        glColor3f(color.x, color.y, color.z);
        glEnable(GL_TEXTURE_2D);

        for (char c : text) {
            const Character& ch = chars.at(c);
            float xpos = x + ch.bearing.x;
            float ypos = -(ch.size.y - ch.bearing.y);
            float w = ch.size.x;
            float h = ch.size.y;

            glBindTexture(GL_TEXTURE_2D, ch.texture_id);
            glBegin(GL_QUADS);
            glTexCoord2f(0, 1); glVertex2f(xpos, ypos);
            glTexCoord2f(1, 1); glVertex2f(xpos + w, ypos);
            glTexCoord2f(1, 0); glVertex2f(xpos + w, ypos + h);
            glTexCoord2f(0, 0); glVertex2f(xpos, ypos + h);
            glEnd();

            x += (ch.advance >> 6);
        }
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

private:
    Font* font;
    std::string text;
    glm::vec3  color;
};
