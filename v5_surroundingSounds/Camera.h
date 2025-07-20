#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <glut.h>
#include <gtc/type_ptr.hpp>

#define PI 3.1415

enum camera_type {
	Perspective,
	Ortho
};

class Camera {
private:
	GLfloat aspect;
	float fovy;
	float z_near;
	float z_far;

	float ortho_bounds[4];

	glm::mat4 perspective_projection{ 1.0f };
	glm::mat4 orthogonal_projection{ 1.0f };

	glm::vec3 position = glm::vec3(0);
	glm::vec3 forward = glm::vec3(0, 0, 1);
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 angle_rotation = glm::vec3(0);

	camera_type type;
	bool dirty = true;

	void screen_reshape(GLfloat height, GLfloat width, GLfloat aspect) {
		if (height == 0)
			height = 1.0f;
		if (height > width)
			aspect = (double)height / (double)width;
		else
			aspect = (double)width / (double)height;
	}

	glm::vec3 calculate_cross(const glm::vec3& v1, const glm::vec3& v2) {
		return glm::vec3(v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	glm::vec3 calculate_normalized_cross(const glm::vec3& v1, const glm::vec3& v2) {
		glm::vec3 res = calculate_cross(v1, v2);
		return glm::normalize(res);
	}

	double round_float(double value, double precision = 0.001)
	{
		return std::round(value / precision) * precision;
	}

	glm::vec3 round_vector(glm::vec3 v)
	{
		v.x = round_float(v.x);
		v.y = round_float(v.y);
		v.z = round_float(v.z);

		return v;
	}
public:

	void set_ortho_dimensions(
		const float& left,
		const float& right,
		const float& bottom,
		const float& top) {
		this->ortho_bounds[0] = left;
		this->ortho_bounds[1] = right;
		this->ortho_bounds[2] = bottom;
		this->ortho_bounds[3] = top;
		this->dirty = true;
	}

	void set_fovy(const float& fovy) {
		this->fovy = fovy;
		this->dirty = true;
	}

	void set_z_near(const float& z_near) {
		this->z_near = z_near;
		this->dirty = true;
	}

	void set_z_far(const float& z_far) {
		this->z_far = z_far;
		this->dirty = true;
	}

	void set_position(const glm::vec3& position) { this->position = position; }

	void set_rotation(const glm::vec3& rotation) { this->angle_rotation = rotation; }

	glm::vec3& get_position() { return this->position; }

	glm::vec3& get_rotation() { return this->angle_rotation; }

	glm::vec3& get_forward() { return this->forward; }

	glm::vec3& get_right() { return this->right; }

	glm::vec3& get_up() { return this->up; }

	camera_type& get_camera_type() { return this->type; }

	glm::mat4& get_perspective_projection() {
		if (this->dirty) {
			this->perspective_projection = glm::perspective(glm::radians(fovy), aspect, z_near, z_far);
			this->dirty = false;
		}
		return this->perspective_projection;
	}

	glm::mat4& get_orthogonal_projection() {
		if (this->dirty) {
			this->orthogonal_projection = glm::ortho(ortho_bounds[0], ortho_bounds[1], ortho_bounds[2], ortho_bounds[3], z_near, z_far);
			this->dirty = false;
		}
		return this->orthogonal_projection;
	}

	Camera() = default;
	Camera(camera_type type) {
		this->type = type;
		aspect = glutGet(GLUT_WINDOW_WIDTH) / glutGet(GLUT_WINDOW_HEIGHT);
		this->set_z_near(0.1f);
		this->set_z_far(100.0f);

		if (type == Perspective)
			this->set_fovy(45.0f);
		else if (type == Ortho)
			this->set_ortho_dimensions(-5, 5, -5 / this->aspect, 5 / this->aspect);
	}

	Camera(GLdouble	left,
		GLdouble right,
		GLdouble bottom,
		GLdouble top,
		GLdouble z_near,
		GLdouble z_far) {
		this->type = Ortho;
		this->set_ortho_dimensions(left, right, bottom, top);
		this->set_z_near(z_near);
		this->set_z_far(z_far);
	}

	Camera(GLdouble fovy,
		GLdouble aspect,
		GLdouble z_near,
		GLdouble z_far) {
		this->type = Perspective;
		this->set_fovy(fovy);
		this->aspect = aspect;
		this->set_z_near(z_near);
		this->set_z_far(z_far);
	}

	void render() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glMultMatrixf(
			glm::value_ptr(this->type == Perspective
				? this->get_perspective_projection()
				: this->get_orthogonal_projection()));

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			this->position.x, this->position.y, this->position.z,
			this->position.x + this->forward.x, this->position.y + this->forward.y, this->position.z + this->forward.z,
			this->up.x, this->up.y, this->up.z
		);
	}

	void update() {
		this->forward.x = sin((this->angle_rotation.y) * PI / 180) * cos((this->angle_rotation.x) * PI / 180);
		this->forward.y = -sin((this->angle_rotation.x) * PI / 180);
		this->forward.z = -cos((this->angle_rotation.y) * PI / 180) * cos((this->angle_rotation.x) * PI / 180);
		this->right = calculate_normalized_cross((-this->up), this->forward);

		GLfloat width = glutGet(GLUT_WINDOW_WIDTH);
		GLfloat height = glutGet(GLUT_WINDOW_HEIGHT);

		this->aspect = width / height;

		screen_reshape(width, height, aspect);
		glViewport(0, 0, width, height);

		position = round_vector(position);
		angle_rotation = round_vector(angle_rotation);
		forward = round_vector(forward);
		right = round_vector(right);
		up = round_vector(up);
	}

	void change_to_ortho(const float& focal_distance = 1.0f) {
		float half_height = focal_distance * tan(glm::radians(fovy) * 0.5f);
		float half_width = half_height * aspect;
		set_ortho_dimensions(-half_width, half_width, -half_height, half_height);
		type = camera_type::Ortho;
	}

	void change_to_perspective(const float& focal_distance = 1.0f) {
		float half_height = (ortho_bounds[3] - ortho_bounds[2]) * 0.5f;
		fovy = glm::degrees(2.0f * atan(half_height / focal_distance));
		dirty = true;
		type = camera_type::Perspective;
	}
};