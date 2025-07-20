#pragma once
#include "Component.h"

#include <glut.h>
#include <glm.hpp>

class CubeRenderer : public Component {
private:
    float size;
    glm::vec3 color;
    bool is_solid;
public:
    CubeRenderer(float s = 1.0f) : size(s), color(glm::vec3(1.0f, 1.0f, 1.0f)), is_solid(true) {}

    void render() override {
        glColor3f(color.r, color.g, color.b);
        if (is_solid)
            glutSolidCube(size);
        else
            glutWireCube(size);
    }

    void init() override {};

    const float& get_size() const { return this->size; }
    void set_size(const float& s) { this->size = s; }

    const  glm::vec3& get_color() const { return this->color; }
    void set_color(const glm::vec3& c) { this->color = c; }

    const bool& get_is_solid() const { return this->is_solid; }
    void set_is_solid(const bool& s) { this->is_solid = s; }
};