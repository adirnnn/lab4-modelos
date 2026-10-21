#include "triangle.h"

#include "line.h"

void triangle(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c) {
    line(a, b);
    line(b, c);
    line(c, a);
}
