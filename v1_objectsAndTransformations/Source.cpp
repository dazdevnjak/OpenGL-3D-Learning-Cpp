#include "GameObject.h"
#include "Utilities.h"

#include <iostream>
#include <glut.h>
#include <glm.hpp>
#include <string.h>

glm::vec2 window_size = { 600, 600 };
glm::vec2 window_position = { 50, 50 };
glm::vec4 window_color = { 0.2f, 0.4f, 0.65f, 1.0f };

const char* window_title = "basic objects & transformation";

float delta_time = 0;
float previous_time = 0;

int delay = 16;

glm::vec3 camera_pos = { 0.0f, 0.0f, 10.0f };
glm::vec3 camera_target = { 0.0f, 0.0f, 0.0f };
glm::vec3 camera_up = { 0.0f, 1.0f, 0.0f };

float rotation_angle = 1.0f;
float rotation_speed = 100.0f;

float scale_size = 0.1f;
float scaleing_speed = 2.0f;

bool growing = true;

std::vector<std::unique_ptr<GameObject>> objects;

void update(float dt) {

	rotation_angle += glm::radians(rotation_speed * dt);
	glm::quat quanternion = glm::angleAxis(rotation_angle, glm::vec3(0.0f, 1.0f, 0.3f));

	for (auto& name : { "SolidCube","WireCube" }) {
		auto* t = Utilities::find_game_object(objects, name)->get_component<Transform>();
		t->rotation = quanternion;
	}

	const float min_s = 0.5f, max_s = 2.0f;

	scale_size += (growing ? 1.0f : -1.0f) * scaleing_speed * dt;

	if (scale_size >= max_s) {
		scale_size = max_s;
		growing = false;
	}
	else if (scale_size <= min_s) {
		scale_size = min_s;
		growing = true;
	}

	if (auto* cone_t = Utilities::find_game_object(objects, "Cone")
		->get_component<Transform>()) {
		cone_t->scale = glm::vec3(scale_size);
	}

	for (auto& obj : objects) {
		obj->update(dt);
	}
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
	auto solid_cube = std::make_unique<GameObject>("SolidCube");
	solid_cube->add_component<CubeRenderer>(1.5f);
	solid_cube->get_component<CubeRenderer>()->set_color(glm::vec3(0.0f, 1.0f, 0.0f));

	auto wire_cube = std::make_unique<GameObject>("WireCube");
	wire_cube->add_component<CubeRenderer>(1.5f);
	auto wire_cube_renderer = wire_cube->get_component<CubeRenderer>();
	wire_cube_renderer->set_color(glm::vec3(0.0f, 0.0f, 0.0f));
	wire_cube_renderer->set_is_solid(false);

	auto sphere = std::make_unique<GameObject>("Sphere");
	sphere->add_component<SphereRenderer>();
	sphere->get_component<Transform>()->position = glm::vec3(-3.0, 3.0, 0.0);
	auto sphere_renderer = sphere->get_component<SphereRenderer>();
	sphere_renderer->set_color(glm::vec3(0.0f, 0.0f, 1.0f));
	sphere_renderer->set_is_solid(false);

	auto cone = std::make_unique<GameObject>("Cone");
	cone->add_component<ConeRenderer>();
	cone->get_component<Transform>()->position = glm::vec3(3.0, -3.0, -2.0);
	glm::quat quaternion = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	cone->get_component<Transform>()->rotation = quaternion;
	cone->get_component<ConeRenderer>()->set_color(glm::vec3(1.0f, 0.0f, 0.0f));

	objects.push_back(std::move(solid_cube));
	objects.push_back(std::move(wire_cube));
	objects.push_back(std::move(sphere));
	objects.push_back(std::move(cone));
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