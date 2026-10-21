#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "color.h"
#include "framebuffer.h"
#include "triangle.h"
#include "obj_loader.h"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Color currentColor;

// datos del modelo tal como vienen en el obj
std::vector<glm::vec3> vertices;
std::vector<Face> faces;

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

void render() {
    // triangulo de prueba, se reemplaza al cargar el modelo
    triangle(glm::vec3(400, 100, 0), glm::vec3(150, 500, 0), glm::vec3(650, 450, 0));
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    if (!loadOBJ(MODEL_PATH, vertices, faces)) {
        return 1;
    }
    std::cout << "Modelo cargado: " << vertices.size() << " vertices, " << faces.size() << " caras" << std::endl;

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
