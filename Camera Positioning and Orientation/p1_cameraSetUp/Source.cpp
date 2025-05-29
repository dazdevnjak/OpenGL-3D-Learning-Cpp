#include "GameObject.h"
#include "CameraController.h"
#include "Camera.h"
#include "Input.h"
#include "Sprite.h"

PFNGLACTIVETEXTUREARBPROC Material::glActiveTextureARB = nullptr;
PFNGLMULTITEXCOORD2FARBPROC Material::glMultiTexCoord2fARB = nullptr;

glm::vec2 window_size = { 500, 500 };
glm::vec2 window_position = { 50, 50 };
glm::vec4 window_color = { 0.2f, 0.4f, 0.65f, 1.0f };

const char* window_title = "camera movemant & rendering";

float delta_time = 0;
float previous_time = 0;

int delay = 16;

std::unique_ptr<GameObject> skull;
std::unique_ptr<GameObject> rocket;

std::unique_ptr<Camera> camera;

float movemant_speed = 5.0f;
float rotation_speed = 3.0f;

bool is_cursor_locked = false;

void update(float dt) {
	camera_controller::free_camera_movemant(*camera, movemant_speed, rotation_speed, dt);
	camera->update();

	if (Input::get_mouse_button_down(GLUT_LEFT_BUTTON))
		Input::set_cursor_lock(is_cursor_locked = !is_cursor_locked);

	skull->update(dt);
	rocket->update(dt);

	Input::update();
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->render();
	camera_controller::enter_3d_rendering(*camera, *skull);
	camera_controller::enter_2d_rendering(*rocket);

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
	skull = std::make_unique<GameObject>("Model");
	skull->add_component<MeshRenderer>("models/skull/skull_downloadable.obj");

	auto* transform = skull->get_component<Transform>();
	if (transform) {
		transform->position = glm::vec3(0, 0, -3);
		glm::quat quaternion = glm::angleAxis(glm::radians(45.0f), glm::vec3(0, 1, 0));
		transform->rotation = quaternion;
	}

	rocket = std::make_unique<GameObject>("Sprite");
	rocket->add_component<Sprite>("sprites/rocket.png");
	
	auto* t = rocket->get_component<Transform>();
	if (t) {
		t->scale = glm::vec3(0.3f, 0.3f, 0.3f);
	}
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

	camera = std::make_unique<Camera>(60.0f, float(window_size.x) / float(window_size.y), 0.1f, 300.0f);

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

	return 0;
}