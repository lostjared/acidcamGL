#include"gl_window.hpp"
#include"gl_shader.hpp"

#include"glm/glm.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"acidcam/ac.h"
//#include<opencv2/opencv.hpp>
#include<unistd.h>


static constexpr int numVAOs = 1;
static constexpr int numVBOs = 2;
int camera_id = 0;
cv::VideoCapture cap;
std::string filename;


class MainWindow : public glWindow {
    GLuint vao[numVAOs];
    GLuint vbo[numVBOs];
    ShaderProgram program;
    float cameraX, cameraY, cameraZ;
    float cube_x, cube_y, cube_z;
    GLuint mv_loc, proj_loc;
    int width, height;
    float aspect;
    glm::mat4 p_mat, v_mat, m_mat, mv_mat, r_mat;
    GLuint texture;
    float pos_z;
    float color_alpha_r, color_alpha_g, color_alpha_b;
public:

    virtual void init() override {
        createShaderProgram();
        cameraX = 0;
        cameraY = 0;
        cameraZ = 0;
        cube_x = 0.0f;
        cube_y = -2.0f;
        cube_z = 0.0f;
        pos_z = -2.0f;
        color_alpha_r = 0.1;
        color_alpha_g = 0.2;
        color_alpha_b = 0.3;
        
        GLfloat vertices[] = {
            -1.0f, -1.0f, 1.0f, // front face
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f
        };
        
        GLfloat texCoords[] = {
            0, 0, // front
            1, 1,
            0, 1,
            
            0, 0,
            1, 0,
            1, 1,
        };
        
        glGenVertexArrays(1, vao);
        glBindVertexArray(vao[0]);
        glGenBuffers(numVBOs, vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
        
        glfwGetFramebufferSize(win(), &width, &height);
        aspect = (float)width/(float)height;
        
        p_mat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        
        cv::Mat frame;
        
        cap.open(camera_id);
        if(!cap.isOpened()) {
            std::cerr << "Error opening file..\n";
            exit(EXIT_FAILURE);
        }
        cap.read(frame);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
    }
    
    int index = 0;
    bool rotate = false;
    
    virtual void update(double timeval) override {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        program.useProgram();
        mv_loc = glGetUniformLocation(program.id(), "mv_matrix");
        proj_loc = glGetUniformLocation(program.id(),"proj_matrix");
        GLuint samp = glGetUniformLocation(program.id(),"samp");
        GLuint calpha_r = glGetUniformLocation(program.id(),"value_alpha_r");
        GLuint calpha_g = glGetUniformLocation(program.id(),"value_alpha_g");
        GLuint calpha_b = glGetUniformLocation(program.id(),"value_alpha_b");
        GLuint c_index = glGetUniformLocation(program.id(),"index_value");
        GLuint c_tf = glGetUniformLocation(program.id(),"time_f");
        v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, cameraY, -cameraZ));
        m_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
        
            if(rotate == true)
                r_mat = glm::rotate(glm::mat4(1.0f), 16.00f*(float)timeval,glm::vec3(0.0f, 0.0f, 1.0f));
        
        //r_mat = glm::rotate(r_mat, 1.75f*(float)timeval,glm::vec3(1.0f, 0.0f, 0.0f));
        //r_mat = glm::rotate(r_mat, 1.75f*(float)timeval, glm::vec3(0.0f, 0.0f, 1.0f));
        if(rotate == true) {
            m_mat = m_mat * r_mat;
        }
        mv_mat = v_mat * m_mat;
        
        glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
        
        glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0,0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(1);
        
        cv::Mat frame;
        if(!cap.read(frame)) {
            std::cout << "Camera closed...\n";
            exit(EXIT_FAILURE);
        }
        cv::flip(frame, frame, 0);
        
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        color_alpha_r += rand()%100 * 0.01f;
        color_alpha_g += rand()%100 * 0.01f;
        color_alpha_b += rand()%100 * 0.01f;
        
        if(color_alpha_r > 1.5f)
            color_alpha_r = 0.1f;
        if(color_alpha_g > 1.5f)
            color_alpha_g = 0.1f;
        if(color_alpha_b > 1.5f)
            color_alpha_b = 0.1f;
        
        glUniform1i(samp, 0);
        glUniform1f(c_index, (float)index);
        glUniform1f(c_tf, timeval);
        glUniform1f(calpha_r, color_alpha_r);
        glUniform1f(calpha_g, color_alpha_g);
        glUniform1f(calpha_b, color_alpha_b);
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LEQUAL);
        
        glDrawArrays(GL_TRIANGLES,0,6);

    }
    
    GLuint createShaderProgram() {
        if(program.loadProgram("vertex.glsl", "frag.glsl")) {
            return program.id();
        }
        return 0;
    }
    
    void keypress(int key, int scancode, int action, int mode) {
        if(key == GLFW_KEY_ESCAPE)
            exit(EXIT_SUCCESS);
        
        if(action == GLFW_RELEASE) {
            switch(key) {
                case GLFW_KEY_LEFT:
                    if(index > 0)
                        --index;
                    
                    break;
                case GLFW_KEY_RIGHT:
                    if(index < 2)
                        ++index;
                    break;
                case GLFW_KEY_A:
                    rotate = (rotate == true) ? false : true;
                    break;
            }
        }
    }
    
    void resize(int newWidth, int newHeight) {
        aspect = (float)newWidth / (float)newHeight;
        glViewport(0, 0, newWidth, newHeight);
        p_mat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
    }
};

MainWindow main_window;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    main_window.keypress(key, scancode, action, mode);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
    main_window.resize(newWidth, newHeight);
}

int main(int argc, char **argv) {
    
    ac::init();
    int w = 1280, h = 720;
    int cw = 1280, ch = 720;
    int opt = 0;
    int device = 0;
    bool full = false;
    int joy_index = -1;
    
    while((opt = getopt(argc, argv, "i:c:r:d:fhvj:")) != -1) {
        switch(opt) {
            case 'i':
                filename = optarg;
                break;
            case 'h':
                std::cout << "acidcamGL " << " arguments:\n-f fullscreen\n-d capture device\n-r resolution 1920x1080\n-c Camera resolution 1280x720\n-v version\n-h help message\n\n";
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                std::cout << "acidcamGL " << "\nwritten by Jared Bruni\n";
                exit(EXIT_SUCCESS);
                break;
            case 'f':
                full = true;
                break;
            case 'd':
                device = atoi(optarg);
                break;
            case 'r': {
                std::string pos = optarg;
                if(pos.rfind("x") == std::string::npos) {
                    std::cerr << "Invalid format for resolution string...1920x1080 is proper.\n";
                    exit(EXIT_FAILURE);
                }
                std::string left=pos.substr(0, pos.rfind("x"));
                std::string right=pos.substr(pos.rfind("x")+1, pos.length());
                w = atoi(left.c_str());
                h = atoi(right.c_str());
                if(w <= 0 || h <= 0) {
                    std::cerr << "Invalid resolution..\n";
                    exit(EXIT_FAILURE);
                }
                std::cout << "Setting Resolution at: " << w << "x" << h << "\n";
            }
                break;
            case 'c': {
                std::string pos = optarg;
                if(pos.rfind("x") == std::string::npos) {
                    std::cerr << "Invalid format for resolution string...1920x1080 is proper.\n";
                    exit(EXIT_FAILURE);
                }
                std::string left=pos.substr(0, pos.rfind("x"));
                std::string right=pos.substr(pos.rfind("x")+1, pos.length());
                cw = atoi(left.c_str());
                ch = atoi(right.c_str());
                if(w <= 0 || h <= 0) {
                    std::cerr << "Invalid resolution..\n";
                    exit(EXIT_FAILURE);
                }
                std::cout << "Desired Camera Resolution: " << w << "x" << h << "\n";
            }
                break;
            case 'j':
                joy_index = atoi(optarg);
                break;
        }
    }
    if(filename.length()==0) {
        cap.open(device);
        if(!cap.isOpened()) {
            std::cerr << "Could not open capture device...\n";
            exit(EXIT_FAILURE);
        }
        cap.set(cv::CAP_PROP_FRAME_WIDTH, cw);
        cap.set(cv::CAP_PROP_FRAME_HEIGHT, ch);
        cw = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        ch = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    } else {
        cap.open(filename);
        if(!cap.isOpened()) {
            std::cerr << "Error could not open file: " << filename << "\n";
            exit(EXIT_FAILURE);
        }
        cw = cap.get(cv::CAP_PROP_FRAME_WIDTH);
        ch = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    }
    camera_id = device;
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    main_window.create(full, "acidcamGL", w, h);
    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    glfwSetKeyCallback(main_window.win(), key_callback);
    glfwSetWindowSizeCallback(main_window.win(), window_size_callback);

    main_window.loop();
    glfwTerminate();
    return EXIT_SUCCESS;
}
