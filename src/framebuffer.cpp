#include "framebuffer.h"

#include <algorithm>
#include <vector>

// la textura espera exactamente 4 bytes por pixel
static_assert(sizeof(Color) == 4, "Color debe ocupar 4 bytes");

namespace {
    // un color por pixel, guardado fila por fila
    std::vector<Color> framebuffer(SCREEN_WIDTH * SCREEN_HEIGHT);
    Color clearColor(20, 24, 36);
    SDL_Texture* texture = nullptr;
}

void setClearColor(const Color& color) {
    clearColor = color;
}

void clear() {
    // llena toda la pantalla con el color de fondo
    std::fill(framebuffer.begin(), framebuffer.end(), clearColor);
}

void point(int x, int y) {
    // ignora los puntos que quedan fuera de la pantalla
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT) {
        framebuffer[y * SCREEN_WIDTH + x] = currentColor;
    }
}

void renderBuffer(SDL_Renderer* renderer) {
    // la textura se crea una sola vez y se reutiliza en cada frame
    if (texture == nullptr) {
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING,
                                    SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    // copia los pixeles del framebuffer a la textura y la muestra
    SDL_UpdateTexture(texture, nullptr, framebuffer.data(), SCREEN_WIDTH * sizeof(Color));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
