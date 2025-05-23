#pragma once
#include "Component.h"
#include "glut.h"

class CubeRenderer : public MeshRenderer {
    float size;
public:
    CubeRenderer(float s = 1.0f) : size(s) {}
    
    void render() override {
        glutSolidCube(size);
    }
    
    const float& get_size() const { return this->size; }
    void set_size(const float& s) { this->size = s; }
};

class SphereRenderer : public MeshRenderer {
    float radius;
    int slices, stacks;
public:
    SphereRenderer(float r = 1.0f, int sl = 16, int st = 16) : radius(r), slices(sl), stacks(st) {}
    
    void render() override {
        glutSolidSphere(radius, slices, stacks);
    }
    
    const float& get_radius() const { return this->radius; }
    void set_radius(const float& r) { this->radius = r; }
    const int& get_slices() const { return this->slices; }
    void set_slices(const int& sl) { this->slices = sl; }
    const int& get_stacks() const { return this->stacks; }
    void set_stacks(const int& st) { this->stacks = st; }
};

class ConeRenderer : public MeshRenderer {
    float base, height;
    int slices, stacks;
public:
    ConeRenderer(float b = 1.0f, float h = 2.0f, int sl = 16, int st = 16) : base(b), height(h), slices(sl), stacks(st) {}
    
    void render() override {
        glutSolidCone(base, height, slices, stacks);
    }

    const float& get_base() const { return this->base; }
    void set_base(const float& b) { this->base = b; }
    const float& get_height() const { return this->height; }
    void set_height(const float& h) { this->height = h; }
    const int& get_slices() const { return this->slices; }
    void set_slices(const int& sl) { this->slices = sl; }
    const int& get_stacks() const { return this->stacks; }
    void set_stacks(const int& st) { this->stacks = st; }
};