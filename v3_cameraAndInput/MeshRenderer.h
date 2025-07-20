#pragma once
#include "Mesh.h"
#include "Component.h"

class MeshRenderer : public Component {
    std::vector<Mesh> meshes;
public:
    MeshRenderer(const std::string& path) {
        meshes = load_meshes(path);
    }

    void render() override {
        for (const auto& mesh : meshes) {

            mesh.material.apply();
            mesh.material.handle_transparency();

            if (!mesh.material.diffuse_map) {
                glDisable(GL_LIGHTING);
                glColor4fv(mesh.material.diffuse);
            }

            glBegin(GL_TRIANGLES);
            for (const auto& v : mesh.vertices) {
                glNormal3f(v.normal.x, v.normal.y, v.normal.z);
                Material::glMultiTexCoord2fARB(GL_TEXTURE0, v.texcoord.x, v.texcoord.y);
                Material::glMultiTexCoord2fARB(GL_TEXTURE1, v.texcoord.x, v.texcoord.y);
                glVertex3f(v.position.x, v.position.y, v.position.z);
            }

            glEnd();
            mesh.material.cleanup();
        }
    }
};