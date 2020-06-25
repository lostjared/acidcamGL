#ifndef __GL_SHADER__
#define __GL_SHADER__

#include<iostream>
#include<string>
#include<GL/glew.h>

class ShaderProgram {
public:
    int printShaderLog(GLuint err);
    void printProgramLog(int p);
    bool checkError();
    GLuint createProgram(const char *vert, const char *frag);
    GLuint createProgramFromFile(const std::string &vert, const std::string &frag);
};

#endif
