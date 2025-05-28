#include "GameObject.h"
#include <iostream>
#include <glut.h>
#include <glm.hpp>
#include <string.h>

glm::vec2 window_size = { 500, 500 };
glm::vec2 window_position = { 50, 50 };
glm::vec4 window_color = { 0.2f, 0.4f, 0.65f, 1.0f };

const char* window_title = "3d basic objects & transformation";

float delta_time = 0;
float previous_time = 0;

int delay = 16;

glm::vec3 camera_pos = { 0.0f, 0.0f, 10.0f };
glm::vec3 camera_target = { 0.0f, 0.0f, 0.0f };
glm::vec3 camera_up = { 0.0f, 1.0f, 0.0f };

std::vector<std::unique_ptr<GameObject>> objects;

float rotation_angle = 0;
float rotation_speed = 90.0f;

void update(float dt) {

	rotation_angle += rotation_speed * dt;
	rotation_angle = std::fmod(rotation_angle, 360.0f);

	if (rotation_angle < 0.0f)
		rotation_angle += 360.0f;

	auto* transform = objects[0]->get_component<Transform>();
	if (transform) {
		glm::quat quaternion = glm::angleAxis(glm::radians(rotation_angle), glm::vec3(0.0f, 1.0f, 0.3f));
		transform->rotation = quaternion;
	}

	for (auto& obj : objects)
		obj->update(dt);
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

	for (auto& obj : objects)
		obj->render();

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
	auto cube = std::make_unique<GameObject>("Cube");
	cube->add_component<CubeRenderer>(1.5f);

	objects.push_back(std::move(cube));
}

void init_game() {
	glClearColor(window_color.r, window_color.g, window_color.b, window_color.a);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, float(window_size.x) / float(window_size.y), 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);

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