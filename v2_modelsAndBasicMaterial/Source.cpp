#include "GameObject.h"
#include "Utilities.h"

#include <iostream>
#include <glut.h>
#include <glm.hpp>
#include <string.h>
#include "Font.h"
#include "Sprite.h"
#include "Text.h"

PFNGLACTIVETEXTUREARBPROC Material::glActiveTextureARB = nullptr;
PFNGLMULTITEXCOORD2FARBPROC Material::glMultiTexCoord2fARB = nullptr;

glm::vec2 window_size = { 600, 600 };
glm::vec2 window_position = { 50, 50 };
glm::vec4 window_color = { 0.2f, 0.4f, 0.65f, 1.0f };

const char* window_title = "models & basic matrials";

float delta_time = 0;
float previous_time = 0;

int delay = 16;

glm::vec3 camera_pos = { 0.0f, 0.0f, 10.0f };
glm::vec3 camera_target = { 0.0f, 0.0f, 0.0f };
glm::vec3 camera_up = { 0.0f, 1.0f, 0.0f };

std::unique_ptr<GameObject> skull;
std::unique_ptr<GameObject> rocket;
std::unique_ptr<GameObject> text;

static std::unique_ptr<Font> shared_font;

void update(float dt) {
	skull->update(dt);
	rocket->update(dt);
	text->update(dt);
}
void render_2d() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glMultMatrixf(glm::value_ptr(glm::ortho(0.0f, (float)glutGet(GLUT_WINDOW_WIDTH), 0.0f, (float)glutGet(GLUT_WINDOW_HEIGHT), -1.0f, 1.0f)));

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_CULL_FACE);

	auto* tf = rocket->get_component<Transform>();
	tf->rotation = glm::quat{ 1,0,0,0 };
	rocket->render();

	auto* tf2 = text->get_component<Transform>();
	tf2->rotation = glm::quat{ 1,0,0,0 };
	text->render();

	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);

	glPopAttrib();
}

void render_3d() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_CULL_FACE);

	skull->render();

	glPopMatrix();

	glPopAttrib();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(
		camera_pos.x, camera_pos.y, camera_pos.z,
		camera_target.x, camera_target.y, camera_target.z,
		camera_up.x, camera_up.y, camera_up.z
	);

	render_3d();
	render_2d();

	glutSwapBuffers();
}

void game_loop() {
	float current_time = glutGet(GLUT_ELAPSED_TIME);
	delta_time = (float)((current_time - previous_time) / 1000);
	previous_time = current_time;

	update(delta_time);
	render();
}

void init_glut(int argv, char** argc) {
	glutInit(&argv, argc);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_size.x, window_size.y);
	glutInitWindowPosition(window_position.x, window_position.y);
	glutCreateWindow(window_title);
}

void init_variables() {
	skull = std::make_unique<GameObject>("Skull");
	skull->add_component<MeshRenderer>("models/skull/skull_downloadable.obj");

	auto* t = skull->get_component<Transform>();
	t->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
	t->position = glm::vec3(0.0f, 0.0f, 5.0f);

	rocket = std::make_unique<GameObject>("Sprite");
	rocket->add_component<Sprite>("sprites/rocket.png");

	auto* t2 = rocket->get_component<Transform>();
	if (t2) {
		t2->scale = glm::vec3(0.3f, 0.3f, 0.3f);
	}

	shared_font = std::make_unique<Font>(
		"fonts/Roboto-Regular.ttf",
		24
	);

	text = std::make_unique<GameObject>("Text");
	text->add_component<Text>(
		shared_font.get(),
		"Print this!",
		glm::vec3(1.0f, 0.84f, 0.0f)
	);
	text->get_component<Transform>()->position = glm::vec3(250.0f, 550.0f, 0.0f);
}

void init_light() {
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat light_pos[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_pos[4] = { 1, 1, 1, 1 };
	GLfloat ambient_pos[4] = { 0.2f, 0.2f, 0.2f, 1 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_pos);
	glLightfv(GL_LIGHT0, GL_SPECULAR, diffuse_pos);
}

void init_game() {
	glClearColor(window_color.r, window_color.g, window_color.b, window_color.a);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, float(window_size.x) / float(window_size.y), 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

	init_light();
	init_variables();
}

void timer(int val) {
	glutPostRedisplay();
	glutTimerFunc(delay, timer, 0);
}

int main(int argv, char** argc) {

	init_glut(argv, argc);

	init_game();

	glutDisplayFunc(game_loop);
	glutTimerFunc(delay, timer, 0);

	glutMainLoop();

	return 0;
}