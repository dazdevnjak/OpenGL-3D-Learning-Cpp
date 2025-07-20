#pragma once
#include "Component.h"

#include <glut.h>
#include <glm.hpp>

class ConeRenderer : public Component {
private:
    float base, height;
    int slices, stacks;
    glm::vec3 color;
    bool is_solid;
public:
    ConeRenderer(float b = 1.0f, float h = 2.0f, int sl = 16, int st = 16) : base(b), height(h), slices(sl), stacks(st), color(glm::vec3(1.0f, 1.0f, 1.0f)), is_solid(true) {}

    void render() override {
        glColor3f(color.r, color.g, color.b);
        if (is_solid)
            glutSolidCone(base, height, slices, stacks);
        else
            glutWireCone(base, height, slices, stacks);
    }

    void init() override {};

    const float& get_base() const { return this->base; }
    void set_base(const float& b) { this->base = b; }
    const float& get_height() const { return this->height; }
    void set_height(const float& h) { this->height = h; }
    const int& get_slices() const { return this->slices; }
    void set_slices(const int& sl) { this->slices = sl; }
    const int& get_stacks() const { return this->stacks; }
    void set_stacks(const int& st) { this->stacks = st; }
    const  glm::vec3& get_color() const { return this->color; }
    void set_color(const glm::vec3& c) { this->color = c; }
    const bool& get_is_solid() const { return this->is_solid; }
    void set_is_solid(const bool& s) { this->is_solid = s; }
};