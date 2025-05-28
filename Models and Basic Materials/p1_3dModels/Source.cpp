#include "GameObject.h"

PFNGLACTIVETEXTUREARBPROC Material::glActiveTextureARB = nullptr;
PFNGLMULTITEXCOORD2FARBPROC Material::glMultiTexCoord2fARB = nullptr;

glm::vec2 window_size = { 500, 500 };
glm::vec2 window_position = { 50, 50 };
glm::vec4 window_color = { 0.2f, 0.4f, 0.65f, 1.0f };

const char* window_title = "3d models & basic matrials";

float delta_time = 0;
float previous_time = 0;

int delay = 16;

glm::vec3 camera_pos = { -3.0f, 1.0f, -3.0f };
glm::vec3 camera_target = { 0.0f, 0.0f, 0.0f };
glm::vec3 camera_up = { 0.0f, 1.0f, 0.0f };

std::vector<std::unique_ptr<GameObject>> objects;

void update(float dt) {
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
	auto mesh = std::make_unique<GameObject>("Mesh");
	mesh->add_component<MeshRenderer>("models/skull/skull_downloadable.obj");

	objects.push_back(std::move(mesh));
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
	gluPerspective(60.0f, float(window_size.x) / float(window_size.y), 0.1f, 300.0f);
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