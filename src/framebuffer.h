#pragma once

#include <SDL2/SDL.h>
#include "color.h"

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

// color con el que point pinta, se define en main.cpp
extern Color currentColor;

void setClearColor(const Color& color);
void clear();
void point(int x, int y);
void renderBuffer(SDL_Renderer* renderer);
