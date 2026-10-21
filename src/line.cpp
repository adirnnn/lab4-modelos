#include "line.h"

#include <cmath>
#include <cstdlib>
#include "framebuffer.h"

void line(const glm::vec3& start, const glm::vec3& end) {
    // bresenham trabaja con pixeles enteros, se redondea cada extremo
    int x0 = static_cast<int>(std::lround(start.x));
    int y0 = static_cast<int>(std::lround(start.y));
    int x1 = static_cast<int>(std::lround(end.x));
    int y1 = static_cast<int>(std::lround(end.y));

    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);
    int stepX = x0 < x1 ? 1 : -1;
    int stepY = y0 < y1 ? 1 : -1;
    int error = dx - dy;

    while (true) {
        // pinta el punto actual antes de avanzar al siguiente
        point(x0, y0);

        if (x0 == x1 && y0 == y1) {
            break;
        }

        // el error decide si se avanza en x, y o en ambos
        int doubleError = 2 * error;

        if (doubleError > -dy) {
            error -= dy;
            x0 += stepX;
        }

        if (doubleError < dx) {
            error += dx;
            y0 += stepY;
        }
    }
}
