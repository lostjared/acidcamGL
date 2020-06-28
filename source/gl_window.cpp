
#include"gl_window.hpp"

int glWindow::create(bool full, std::string name, int w, int h) {
#ifdef __APPLE__
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
    window = glfwCreateWindow(w, h, name.c_str(),(full == true) ? glfwGetPrimaryMonitor() : 0,0);
    if(!window) return 0;
    glfwMakeContextCurrent(window);
    if(glewInit()!=GLEW_OK)
        exit(EXIT_FAILURE);
    glfwSwapInterval(1);
    init();
    return 1;
}

void glWindow::loop() {
    while(!glfwWindowShouldClose(window)) {
        update(glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}

