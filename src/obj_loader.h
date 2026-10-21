#pragma once

#include <array>
#include <string>
#include <vector>
#include <glm/glm.hpp>

// una cara guarda los indices (desde 0) de sus tres vertices
struct Face {
    std::array<int, 3> vertexIndices;
};

// lee los vertices y caras de un archivo obj, devuelve false si algo falla
bool loadOBJ(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<Face>& faces);
