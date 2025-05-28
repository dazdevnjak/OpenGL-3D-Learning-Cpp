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

    Mesh(const std::string& n = "Mesh") : name(n) { }

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

        std::string line;
        std::string name;
        while (std::getline(file, line)) {
            if (line.empty())
                continue;

            std::istringstream iss(line);
            std::string key;
            iss >> key;
        
            if (key[0] == '#') continue;

            if (key == "newmtl") {
                iss >> name;
                materials[name].name = name;
            }
            else if (key == "Ka") {
                auto& a = materials[name].ambient;
                iss >> a[0] >> a[1] >> a[2];
                a[3] = materials[name].diffuse[3];
            }
            else if (key == "Kd") {
                auto& d = materials[name].diffuse;
                iss >> d[0] >> d[1] >> d[2];
                d[3] = materials[name].diffuse[3];
            }
            else if (key == "d" || key == "Tr") {
                float alpha;
                iss >> alpha;

                if (key == "Tr") 
                    alpha = 1.0f - alpha;
                
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
        if (line.empty())
            continue;

        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key[0] == '#') continue;

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
            std::string new_material;
            iss >> new_material;
            if (new_material != current_material) {
                if (started && !current_mesh.vertices.empty()) {
                    meshes.push_back(std::move(current_mesh));
                    current_mesh = Mesh();
                }
            }
            current_material = new_material;
            if (materials.count(current_material))
                current_mesh.material = materials[current_material];
        }
        else if (key == "v") {
            glm::vec3 v;
            iss >> v.x >> v.y >> v.z;
            positions.push_back(v);
        }
        else if (key == "vt") {
            glm::vec2 vt;
            iss >> vt.x >> vt.y;
            texcoords.push_back(vt);
        }
        else if (key == "vn") {
            glm::vec3 n;
            iss >> n.x >> n.y >> n.z;
            normals.push_back(n);
        }
        else if (key == "f") {
            std::string token;

            while (iss >> token) {

                int vi = 0, ti = 0, ni = 0;
                char slash = '/';
                std::istringstream ss(token);

                if (!(ss >> vi >> slash >> ti >> slash >> ni)) {
                    std::cerr << "Maflormed face token: "
                        << token << ". Token skiped\n";
                    continue;
                }

                Vertex vertex;
                vertex.position = positions[vi - 1];
                vertex.texcoord = texcoords[ti - 1];
                vertex.normal = normals[ni - 1];

                current_mesh.vertices.push_back(vertex);
            }
        }
    };

    if (started && !current_mesh.vertices.empty())
        meshes.push_back(std::move(current_mesh));

    file.close();
    return meshes;
}