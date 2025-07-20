#pragma once
#include "Component.h"

#include <glut.h>
#include <glm.hpp>

class SphereRenderer : public Component {
private:
    float radius;
    int slices, stacks;
    glm::vec3 color;
    bool is_solid;
public:
    SphereRenderer(float r = 1.0f, int sl = 16, int st = 16) : radius(r), slices(sl), stacks(st), color(glm::vec3(1.0f, 1.0f, 1.0f)), is_solid(true) {}

    void render() override {
        glColor3f(color.r, color.g, color.b);
        if (is_solid)
            glutSolidSphere(radius, slices, stacks);
        else
            glutWireSphere(radius, slices, stacks);
    }

    void init() override {};

    const float& get_radius() const { return this->radius; }
    void set_radius(const float& r) { this->radius = r; }
    const int& get_slices() const { return this->slices; }
    void set_slices(const int& sl) { this->slices = sl; }
    const int& get_stacks() const { return this->stacks; }
    void set_stacks(const int& st) { this->stacks = st; }
    const  glm::vec3& get_color() const { return this->color; }
    void set_color(const glm::vec3& c) { this->color = c; }
    const bool& get_is_solid() const { return this->is_solid; }
    void set_is_solid(const bool& s) { this->is_solid = s; }
};