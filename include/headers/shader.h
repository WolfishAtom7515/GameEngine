#pragma once

#include "glad.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class shader {
private:
    GLuint id;
public:
    GLuint getId() const;

    void use() const;

    shader(const char* vertexPath, const char* fragmentPath);
    ~shader();
};
