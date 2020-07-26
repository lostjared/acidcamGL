#ifndef __GL_WINDOW__H_
#define __GL_WINDOW__H_

#include<iostream>
#include<string>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

namespace acidcam {
    class glWindow {
    public:
        int create(bool record, bool full, bool resize, std::string name, int w, int h, int monitor);
        virtual void init() = 0;
        virtual void update(double timeval) = 0;
        void loop();
        GLFWwindow *win() const { return window; }
    private:
        GLFWwindow *window;
    protected:
        bool active;
        int window_width, window_height;
    };
    
    bool checkForError();
}

#endif

