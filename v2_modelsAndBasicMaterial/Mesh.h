#pragma once
#include "Material.h"
#include "Texture.h"

#include <glut.h>

#include <glm.hpp>

#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "array"
#include <string>
#include <filesystem>

struct Vertex {
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
};

class Mesh {
public:
    std::string name;
    std::vector<Vertex> vertices;
    Material material;

    Mesh(const std::string& n = "Mesh") : name(n) {}

    static void load_mtl(const std::string& path, std::map<std::string, Material>& materials) {
        if (!file_exist(path)) {
            std::cerr << "Material not found at: " << path << std::endl;
            return;
        }

        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Cannot open material file at: " << path << std::endl;
            return;
        }

        std::string line, name;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            std::string key;
            iss >> key;

            if (key == "newmtl") {
                iss >> name;
                materials[name].name = name;
            }
            else if (key == "Ka") {
                iss >> materials[name].ambient[0]
                    >> materials[name].ambient[1]
                    >> materials[name].ambient[2];
            }
            else if (key == "Kd") {
                iss >> materials[name].diffuse[0]
                    >> materials[name].diffuse[1]
                    >> materials[name].diffuse[2];
            }
            else if (key == "d" || key == "Tr") {
                float alpha; iss >> alpha;
                if (key == "Tr") alpha = 1.0f - alpha;
                materials[name].diffuse[3] = alpha;
                materials[name].ambient[3] = alpha;
            }
            else if (key == "map_Kd") {
                std::string fn; iss >> fn;
                std::string dir = path.substr(0, path.find_last_of("/\\") + 1);
                materials[name].diffuse_map = load_texture(dir + fn);
            }
            else if (key == "bump" || key == "map_Bump" || key == "norm") {
                std::string fn; iss >> fn;
                std::string dir = path.substr(0, path.find_last_of("/\\") + 1);
                materials[name].normal_map = load_texture(dir + fn);
            }
        }
        file.close();
    }
};

static std::vector<Mesh> load_meshes(const std::string& path) {
    std::vector<Mesh> meshes;

    if (!file_exist(path)) {
        std::cerr << "Mesh not found at: " << path << std::endl;
        return meshes;
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open mesh file at: " << path << std::endl;
        return meshes;
    }

    Mesh current_mesh;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;

    size_t dot_pos = path.find_last_of('.');
    std::string mtl_path = path.substr(0, dot_pos) + ".mtl";

    std::string current_material;
    std::map<std::string, Material> materials;

    Mesh::load_mtl(mtl_path, materials);

    bool started = false;
    std::string line;
    while (std::getline(file, line)) {

        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string key; iss >> key;

        if (key == "o") {
            if (started && !current_mesh.vertices.empty())
                meshes.push_back(std::move(current_mesh));

            std::string name;
            iss >> name;

            current_mesh = Mesh(name);
            current_material.clear();

            started = true;
        }
        else if (key == "usemtl") {
            iss >> current_material;
            if (materials.count(current_material)) current_mesh.material = materials[current_material];
        }
        else if (key == "v") {
            glm::vec3 v; iss >> v.x >> v.y >> v.z;
            positions.push_back(v);
        }
        else if (key == "vt") {
            glm::vec2 vt; iss >> vt.x >> vt.y;
            texcoords.push_back(vt);
        }
        else if (key == "vn") {
            glm::vec3 n; iss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (key == "f") {
            std::vector<std::array<int, 3>> face;
            std::string tok;
            while (iss >> tok) {
                std::array<int, 3> idx = { 0,0,0 }; size_t pos = 0; int p = 0;
                while (p < 3) {
                    size_t slash = tok.find('/', pos);
                    std::string piece = slash == std::string::npos
                        ? tok.substr(pos)
                        : tok.substr(pos, slash - pos);
                    if (!piece.empty()) idx[p] = std::stoi(piece);
                    if (slash == std::string::npos) break;
                    pos = slash + 1; ++p;
                }
                face.push_back(idx);
            }
            for (size_t i = 1; i + 1 < face.size(); ++i) {
                std::array<std::array<int, 3>, 3> tri = { face[0], face[i], face[i + 1] };
                for (auto const& f : tri) {
                    Vertex vtx{};
                    if (f[0] > 0 && f[0] <= (int)positions.size()) vtx.position = positions[f[0] - 1];
                    if (f[1] > 0 && f[1] <= (int)texcoords.size()) vtx.texcoord = texcoords[f[1] - 1];
                    if (f[2] > 0 && f[2] <= (int)normals.size())   vtx.normal = normals[f[2] - 1];
                    current_mesh.vertices.push_back(vtx);
                }
            }
        }
    };

    if (started && !current_mesh.vertices.empty())
        meshes.push_back(std::move(current_mesh));

    file.close();
    return meshes;
}