#include "obj_loader.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {
    // convierte un elemento como "12/5/3" en el indice del vertice desde 0
    bool parseVertexIndex(const std::string& token, int vertexCount, int& index) {
        std::istringstream stream(token.substr(0, token.find('/')));
        int value = 0;
        if (!(stream >> value) || value == 0) {
            return false;
        }

        // obj empieza en 1, los negativos cuentan desde el ultimo vertice leido
        index = value > 0 ? value - 1 : vertexCount + value;
        return index >= 0 && index < vertexCount;
    }
}

bool loadOBJ(const std::string& path, std::vector<glm::vec3>& vertices, std::vector<Face>& faces) {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "No se pudo abrir " << path << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::istringstream stream(line);
        std::string type;
        stream >> type;

        if (type == "v") {
            glm::vec3 vertex;
            if (!(stream >> vertex.x >> vertex.y >> vertex.z)) {
                std::cerr << "Vertice invalido en la linea " << lineNumber << std::endl;
                return false;
            }
            vertices.push_back(vertex);
        } else if (type == "f") {
            std::vector<int> indices;
            std::string token;
            while (stream >> token) {
                int index = 0;
                if (!parseVertexIndex(token, static_cast<int>(vertices.size()), index)) {
                    std::cerr << "Cara invalida en la linea " << lineNumber << std::endl;
                    return false;
                }
                indices.push_back(index);
            }

            if (indices.size() < 3) {
                std::cerr << "Cara con menos de 3 vertices en la linea " << lineNumber << std::endl;
                return false;
            }

            // si la cara tiene mas de 3 vertices se divide en triangulos tipo abanico
            for (size_t i = 1; i + 1 < indices.size(); i++) {
                faces.push_back(Face{{indices[0], indices[i], indices[i + 1]}});
            }
        }
    }

    return true;
}
