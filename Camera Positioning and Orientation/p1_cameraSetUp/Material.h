#pragma once
#include <windows.h>
#include <GL/gl.h>
#include <glut.h>
#include <iostream>

#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <glext.h>

typedef void (APIENTRY* PFNGLACTIVETEXTUREARBPROC)(GLenum texture);
typedef void (APIENTRY* PFNGLMULTITEXCOORD2FARBPROC)(GLenum target, GLfloat s, GLfloat t);

class Material {
public:
	std::string name;

	GLfloat ambient[4];
	GLfloat diffuse[4];

	GLuint diffuse_map;
	GLuint normal_map;

	static PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
	static PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB;

	Material(const std::string& n = "Material")
		: diffuse_map(0), normal_map(0), name(n)
	{
		load_uv_func();
		ambient[0] = ambient[1] = ambient[2] = 1.0f; ambient[3] = 1.0f;
		diffuse[0] = diffuse[1] = diffuse[2] = 1.0f; diffuse[3] = 1.0f;
	}

	void handle_transparency() const {
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.5f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void apply() const {
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);

		glActiveTextureARB(GL_TEXTURE0);
		if (normal_map) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, normal_map);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_DOT3_RGB);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
		}
		else {
			glDisable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PRIMARY_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
		}

		glActiveTextureARB(GL_TEXTURE1);
		if (diffuse_map) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, diffuse_map);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
		}
		else {
			glDisable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
			glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_PREVIOUS);
			glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
		}

		glActiveTextureARB(GL_TEXTURE0);
	}

	void cleanup() const {

		for (int u = 0; u < 2; ++u) {
			glActiveTextureARB(GL_TEXTURE0 + u);
			glDisable(GL_TEXTURE_2D);
			glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		}
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
		glDisable(GL_ALPHA_TEST);
		glActiveTextureARB(GL_TEXTURE0);
	}
private:
	void load_uv_func() {
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		if (!glActiveTextureARB) {
			glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTexture");
		}
		if (!glActiveTextureARB) {
			std::cerr << "Fatal Error: glActiveTextureARB not found!\n";
			exit(EXIT_FAILURE);
		}

		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
		if (!glMultiTexCoord2fARB) {
			glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2f");
		}
		if (!glMultiTexCoord2fARB) {
			std::cerr << "Fatal Error: glMultiTexCoord2fARB not found!\n";
			exit(EXIT_FAILURE);
		}
	}
};