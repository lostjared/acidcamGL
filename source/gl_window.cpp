
#include"gl_window.hpp"

namespace acidcam {
    
    int glWindow::create(bool record, bool full, bool resize, std::string name, int w, int h, int monitor) {
#ifdef __APPLE__
        glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        
        int count;
        GLFWmonitor** monitors = glfwGetMonitors(&count);
        if(monitor > count)
            monitor = 0;
        
        if(full == true && resize == false) {
            const GLFWvidmode* mode = glfwGetVideoMode(monitors[monitor]);
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            w = mode->width;
            h = mode->height;
        }
        
        if(record)
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(w, h, name.c_str(),(full == true && record == false) ? monitors[monitor] : 0,0);
        if(!window) return 0;
        glfwMakeContextCurrent(window);
        if(glewInit()!=GLEW_OK)
            exit(EXIT_FAILURE);
        
        window_width = w;
        window_height = h;
        if(record) {
            glfwGetWindowSize(window, &w, &h);
            glfwSetWindowSizeLimits(window, w, h, w, h);
        }
        glfwSwapInterval(1);
        init();
        return 1;
    }
    
    void glWindow::loop() {
        active = true;
        while(!glfwWindowShouldClose(window) && active == true) {
            update(glfwGetTime());
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwDestroyWindow(window);
    }
    
    void glWindow::quit() {
        active = false;
    }
    
    bool checkForError() {
        bool e = false;
        int glErr = glGetError();
        while(glErr != GL_NO_ERROR) {
            std::cout << "acidcam: GL Error: " << glErr << "\n";
            std::cout << "acidcam: Error String: " << glewGetErrorString(glErr) << "\n";
            e = true;
            glErr = glGetError();
        }
        return e;
    }
}
