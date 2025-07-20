#include "SoundManager.h"
#include "GameObject.h"
#include "Utilities.h"
#include "Camera.h"
#include "Input.h"

#include <iostream>
#include <glut.h>
#include <glm.hpp>
#include <string.h>
#include "Sprite.h"
#include "Text.h"

#include "DebugDrawer.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "btBulletDynamicsCommon.h"
#include "MotionStateObject.h"

PFNGLACTIVETEXTUREARBPROC Material::glActiveTextureARB = nullptr;
PFNGLMULTITEXCOORD2FARBPROC Material::glMultiTexCoord2fARB = nullptr;

glm::vec2 window_size = { 600, 600 };
glm::vec2 window_position = { 50, 50 };
glm::vec4 window_color = { 0.2f, 0.4f, 0.65f, 1.0f };

const char* window_title = "surrounding sounds";

float delta_time = 0;
float previous_time = 0;

int delay = 16;

float movement_speed = 5.0f;
float rotation_speed = 3.0f;

bool is_cursor_locked = false;

std::unique_ptr<Camera> camera;

std::unique_ptr<GameObject> skull;
std::unique_ptr<GameObject> skull1;
std::unique_ptr<GameObject> rocket;
std::unique_ptr<GameObject> text;

static std::unique_ptr<Font> shared_font;

btBroadphaseInterface* broadphase;
btCollisionConfiguration* collision_configuration;
btCollisionDispatcher* dispatcher;
btConstraintSolver* solver;
btDynamicsWorld* world;

GLDebugDrawer* debug_drawer;


void move_camera() {
	float v = Input::get_axis("Vertical");
	float h = Input::get_axis("Horizontal");

	glm::vec3 movemant =
		camera->get_forward() * v * movement_speed * delta_time
		+ camera->get_right() * h * movement_speed * delta_time;

	camera->set_position(
		camera->get_position() + movemant
	);

	float x = 0.0, y = 0.0;
	Input::get_mouse_position(&x, &y);

	float yaw = camera->get_rotation().y + float(-x * rotation_speed * delta_time);
	float pitch = camera->get_rotation().x + float(-y * rotation_speed * delta_time);

	pitch = glm::clamp(pitch, -359.0f, 359.0f);

	camera->set_rotation(glm::vec3(pitch, yaw, 0.0f));
}

void update_physics(float delta_time) {

	world->stepSimulation(delta_time);
}

void update(float dt) {

	move_camera();
	camera->update();

	if (Input::get_key_down('E'))
		Input::set_cursor_lock(is_cursor_locked = !is_cursor_locked);

	if (Input::get_key_down('R'))
		SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().first_sound, SoundManager::get_instance().surrounding_sounds, glm::vec3(4, 0, 4));
	else if (Input::get_key_down('T'))
		SoundManager::get_instance().play_sound_on_position(SoundManager::get_instance().second_sound, SoundManager::get_instance().surrounding_sounds, glm::vec3(1, 0, 1));

	skull->update(dt);
	skull1->update(dt);
	rocket->update(dt);
	text->update(dt);

	SoundManager::get_instance().update();
	SoundManager::get_instance().update_listener(camera->get_position(), camera->get_forward(), camera->get_up());
	Input::update();
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
	skull1->render();

	world->debugDrawWorld();

	glPopMatrix();

	glPopAttrib();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	camera->render();

	render_3d();
	render_2d();

	glutSwapBuffers();
}

void game_loop() {
	float current_time = glutGet(GLUT_ELAPSED_TIME);
	delta_time = (float)((current_time - previous_time) / 1000);
	previous_time = current_time;

	update(delta_time);
	update_physics(delta_time);
	render();
}

void init_physics()
{
	collision_configuration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_configuration);
	broadphase = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collision_configuration);

	debug_drawer = new GLDebugDrawer();
	debug_drawer->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	world->setDebugDrawer(debug_drawer);
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
	t->position = glm::vec3(0.0f, 0.0f, -3.0f);

	auto* shape = new btBoxShape(btVector3(1, 1, 1));
	skull->add_component<RigidBody>(1.0f, shape, world);

	skull1 = std::make_unique<GameObject>("Skull");
	skull1->add_component<MeshRenderer>("models/skull/skull_downloadable.obj");

	auto* t1 = skull1->get_component<Transform>();
	t1->rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
	t1->position = glm::vec3(0.0f, -5.0f, -3.0f);

	auto* shape1 = new btBoxShape(btVector3(1, 1, 1));
	skull1->add_component<RigidBody>(0.0f, shape1, world);

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
	init_physics();

	camera = std::make_unique<Camera>(60.0f, float(window_size.x) / float(window_size.y), 0.1f, 300.0f);

	SoundManager::get_instance().init_fmod();
	SoundManager::get_instance().init_audio();

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

	Input::init();

	glutMainLoop();
	SoundManager::get_instance().release();

	return 0;
}