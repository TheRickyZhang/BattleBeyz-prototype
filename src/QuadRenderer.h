# pragma once

#include <GL/glew.h>

class QuadRenderer {
public:
    QuadRenderer();
    ~QuadRenderer();

    void setup();
    void render() const;

private:
    unsigned int quadVAO, quadVBO;
};