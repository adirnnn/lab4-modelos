#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <vector>
#include "color.h"
#include "framebuffer.h"
#include "triangle.h"
#include "obj_loader.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Color currentColor;

// rotacion fija para ver la nave en tres cuartos y no completamente de frente
constexpr float MODEL_YAW = -145.0f;
constexpr float MODEL_PITCH = 25.0f;
// fraccion de la pantalla que puede ocupar el modelo
constexpr float MODEL_FILL = 0.7f;

// datos del modelo tal como vienen en el obj
std::vector<glm::vec3> vertices;
std::vector<Face> faces;
// los mismos vertices ya convertidos a coordenadas de pantalla
std::vector<glm::vec3> screenVertices;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "No se pudo iniciar SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    // vsync limita el ciclo a la frecuencia del monitor
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    return window != nullptr && renderer != nullptr;
}

void setColor(const Color& color) {
    currentColor = color;
}

void prepareVertices() {
    // centro de la caja que envuelve al modelo
    glm::vec3 minPoint = vertices[0];
    glm::vec3 maxPoint = vertices[0];
    for (const glm::vec3& vertex : vertices) {
        minPoint = glm::min(minPoint, vertex);
        maxPoint = glm::max(maxPoint, vertex);
    }
    glm::vec3 center = (minPoint + maxPoint) * 0.5f;

    // gira el modelo alrededor de su centro
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(MODEL_PITCH), glm::vec3(1, 0, 0));
    rotation = glm::rotate(rotation, glm::radians(MODEL_YAW), glm::vec3(0, 1, 0));

    std::vector<glm::vec3> rotated;
    for (const glm::vec3& vertex : vertices) {
        rotated.push_back(glm::vec3(rotation * glm::vec4(vertex - center, 1.0f)));
    }

    // caja en x, y despues de girar, es lo que se ve en pantalla
    glm::vec2 minScreen(rotated[0]);
    glm::vec2 maxScreen(rotated[0]);
    for (const glm::vec3& vertex : rotated) {
        minScreen = glm::min(minScreen, glm::vec2(vertex));
        maxScreen = glm::max(maxScreen, glm::vec2(vertex));
    }
    glm::vec2 size = maxScreen - minScreen;
    glm::vec2 middle = (minScreen + maxScreen) * 0.5f;

    // escala para que el modelo quepa en la pantalla sin deformarse
    float scale = std::min(SCREEN_WIDTH * MODEL_FILL / size.x, SCREEN_HEIGHT * MODEL_FILL / size.y);

    // mueve el modelo al centro de la pantalla, y se invierte porque en pantalla crece hacia abajo
    screenVertices.clear();
    for (const glm::vec3& vertex : rotated) {
        screenVertices.push_back(glm::vec3(
            SCREEN_WIDTH / 2.0f + (vertex.x - middle.x) * scale,
            SCREEN_HEIGHT / 2.0f - (vertex.y - middle.y) * scale,
            vertex.z * scale));
    }
}

void render() {
    // recorre cada cara, toma sus tres indices y dibuja el triangulo con esos vertices
    for (const Face& face : faces) {
        glm::vec3 v1 = screenVertices[face.vertexIndices[0]];
        glm::vec3 v2 = screenVertices[face.vertexIndices[1]];
        glm::vec3 v3 = screenVertices[face.vertexIndices[2]];
        triangle(v1, v2, v3);
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    if (!loadOBJ(MODEL_PATH, vertices, faces) || faces.empty()) {
        std::cerr << "No se encontro un modelo para dibujar" << std::endl;
        return 1;
    }
    std::cout << "Modelo cargado: " << vertices.size() << " vertices, " << faces.size() << " caras" << std::endl;
    prepareVertices();

    if (!init()) {
        return 1;
    }

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }

        clear();

        setColor(Color(255, 255, 0));
        render();

        renderBuffer(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
