#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <SOIL2.h>
#include <glut.h>

inline bool file_exist(const std::string& path) {
	std::ifstream file(path);
	return file.good();
}

static GLuint load_texture(const std::string& path) {
	if (!file_exist(path)) {
		std::cerr << "Texture not found at: " << path << std::endl;
		return 0;
	}

	GLuint id = SOIL_load_OGL_texture(
		path.c_str(),
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
	);

	if (id == 0)
		std::cerr << "Failed to load texture at: " << path << ". last result was: " << SOIL_last_result() << std::endl;
	else {
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	return id;
}
