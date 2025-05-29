#pragma once
#include "glut.h"
#include "Texture.h"
#include "Component.h"

class Sprite : public Component {
	GLuint texture_id = 0;
	int width, height = 0;
public:
	std::string name;

	Sprite(const std::string& path, const std::string& n = "Sprite") : name(n) {
		texture_id = load_texture(path);

		if (texture_id != 0) {
			query_texture_size(texture_id, width, height);
		}
		else {
			std::cerr << "Failed to load sprite on path: " << path << std::endl;
		}
	}

	~Sprite() {
		if(texture_id != 0)
			glDeleteTextures(1, &texture_id);
	}

	void render() override {
		glBindTexture(GL_TEXTURE_2D, this->texture_id);

		glPushMatrix();
		
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); 
		glVertex2f(0, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0, height);
		glEnd();

		glPopMatrix();

		glBindTexture(GL_TEXTURE_2D, 0);
	}
};