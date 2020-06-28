#ifndef __GL_SHADER__
#define __GL_SHADER__

#include<iostream>
#include<string>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

namespace acidcam {
    class ShaderProgram {
    public:
        ShaderProgram();
        int printShaderLog(GLuint err);
        void printProgramLog(int p);
        bool checkError();
        GLuint createProgram(const char *vert, const char *frag);
        GLuint createProgramFromFile(const std::string &vert, const std::string &frag);
        
        bool loadProgram(const std::string &text1, const std::string &text2);
        int id() const { return shader_id; }
        void useProgram();
        
    private:
        int shader_id;
    };
}
#endif
