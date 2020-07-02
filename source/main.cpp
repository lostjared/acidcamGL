#include"gl_window.hpp"
#include"gl_shader.hpp"

#include"glm/glm.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtc/matrix_transform.hpp"
#include"acidcam/ac.h"
#include<unistd.h>
#include<vector>
#include<sstream>
#include<string>
#include<fstream>

#define version_info "v1.0"

namespace acidcam {
    
    cv::VideoCapture cap;
    
    class AcidCam_Window : public glWindow {
        static constexpr int numVAOs = 1;
        static constexpr int numVBOs = 2;
        GLuint vao[numVAOs];
        GLuint vbo[numVBOs];
        ShaderProgram program;
        GLuint mv_loc, proj_loc;
        int width, height;
        float aspect;
        glm::mat4 p_mat, v_mat, m_mat, mv_mat;
        GLuint texture;
        float color_alpha_r, color_alpha_g, color_alpha_b;
        std::vector<ShaderProgram> shaders;
        int shader_index;
        bool print_text;
        bool debug;
        float alpha;
    public:
        
        virtual void init() override {
            alpha = 0.1f;
            debug = false;
            print_text = false;
            shader_index = 0;
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
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            cv::Mat frame;
            if(!cap.isOpened()) {
                std::cerr << "Error opening camera/file..\n";
                exit(EXIT_FAILURE);
            }
            cap.read(frame);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
        }
        
        int index = 0;
        bool rotate = false;
        
        void setShader(int index) {
            program = shaders[index];
            if(debug) {
                std::cout << "Shader loaded: " << program.name() << "\n";
            }
        }
        
        void setPrintText(bool b) {
            print_text = b;
        }
        
        virtual void update(double timeval) override {
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClearDepth(1.0);
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
            GLuint alpha_pos = glGetUniformLocation(program.id(), "alpha_value");
            v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
            m_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
            
            mv_mat = v_mat * m_mat;
            
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
            if(shader_index == 0) {
                if(index >= 0 && index < ac::solo_filter.size()-1) {
                    ac::CallFilter(ac::solo_filter[index], frame);
                    if(print_text == true) {
                        std::ostringstream stream;
                        stream << ac::solo_filter[index];
                        cv::putText(frame, stream.str(),cv::Point(40, 40),cv::FONT_HERSHEY_DUPLEX,1.0,CV_RGB(255, 255, 255), 2);
                    }
                }
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
            
            if(program.name()=="scale") {
                static bool idir = true;
                if(idir == true) {
                    alpha += 0.05f;
                    if(alpha >= 10.0)
                        idir = false;
                } else {
                    alpha -= 0.05f;
                    if(alpha <= 0.1f)
                        idir = true;
                }
            }
            glUniform1i(samp, 0);
            glUniform1f(c_index, (float)index);
            glUniform1f(c_tf, timeval);
            glUniform1f(calpha_r, color_alpha_r);
            glUniform1f(calpha_g, color_alpha_g);
            glUniform1f(calpha_b, color_alpha_b);
            glUniform1f(alpha_pos, alpha);
            glDrawArrays(GL_TRIANGLES,0,6);
        }
        
        void setDebug(bool d) {
            debug = d;
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
                        if(index < ac::solo_filter.size()-1)
                            ++index;
                        break;
                    case GLFW_KEY_UP:
                        
                        if(shader_index > 0) {
                            --shader_index;
                            setShader(shader_index);
                        }
                        
                        break;
                    case GLFW_KEY_DOWN:
                        
                        if(shader_index < shaders.size()-1) {
                            shader_index++;
                            setShader(shader_index);
                        }
                        
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
        
        void loadShaders(const std::string &text) {
            std::ostringstream stream;
            stream << text << "/" << "index.txt";
            std::fstream file;
            file.open(stream.str(), std::ios::in) ;
            if(!file.is_open()) {
                std::cerr << "Error could not open: " << stream.str() << "\n";
                exit(EXIT_FAILURE);
            }
            
            while(!file.eof()) {
                std::string s;
                std::getline(file, s);
                if(file) {
                    std::ostringstream fs1,fs2;
                    fs1 << text << "/" << s;
                    fs2 << text << "/vertex.glsl";
                    ShaderProgram p;
                    std::cout << "Compiling [" << s << "] ";
                    if(p.loadProgram(fs2.str(), fs1.str())==false) {
                        std::cerr << "Error could not load: " << fs1.str() << "\n";
                        exit(EXIT_FAILURE);
                    }
                    std::cout << "\n";
                    p.setName(s.substr(0, s.rfind(".")));
                    shaders.push_back(p);
                }
            }
            file.close();
        }
    };
}
acidcam::AcidCam_Window main_window;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    main_window.keypress(key, scancode, action, mode);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
    main_window.resize(newWidth, newHeight);
}

void print_help_message() {
    std::cout << "acidcamGL " << version_info << " arguments:\n-e output debug strings\n-u fps\n-s shader path\n-f fullscreen\n-d capture device\n-r resolution 1920x1080\n-c Camera resolution 1280x720\n-v version\n-h help messagecD\n\n";
}

int main(int argc, char **argv) {
    
    if(argc == 1) {
        print_help_message();
        exit(EXIT_SUCCESS);
    }
    
    if(!glfwInit()) {
        exit(EXIT_FAILURE);
    }
    ac::init();
    std::string filename;
    int w = 1280, h = 720;
    int cw = 1280, ch = 720;
    int opt = 0;
    int device = 0;
    bool full = false;
    int joy_index = -1;
    std::string shader_path;
    bool print_text = false;
    double fps = 24.0;
    bool debug_val = false;
    
    while((opt = getopt(argc, argv, "eu:pi:c:r:d:fhvj:s:")) != -1) {
        switch(opt) {
            case 'e':
                debug_val = true;
                break;
            case 'u':
                fps = atof(optarg);
                break;
            case 'p':
                print_text = true;
                break;
            case 's':
                shader_path = optarg;
                break;
            case 'i':
                filename = optarg;
                break;
            case 'h':
                print_help_message();
                exit(EXIT_SUCCESS);
                break;
            case 'v':
                std::cout << "acidcamGL " << version_info << "\nwritten by Jared Bruni\n";
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
                std::cout << "Setting Window Resolution at: " << w << "x" << h << "\n";
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
                std::cout << "Desired Capture Resolution: " << cw << "x" << ch << "\n";
            }
                break;
            case 'j':
                joy_index = atoi(optarg);
                break;
        }
    }
    if(shader_path.length()==0) {
        std::cerr << "Error: must provide path to shaders...\n";
        exit(EXIT_FAILURE);
    }
    if(filename.length()==0) {
        acidcam::cap.open(device);
        if(!acidcam::cap.isOpened()) {
            std::cerr << "Could not open capture device...\n";
            exit(EXIT_FAILURE);
        }
        acidcam::cap.set(cv::CAP_PROP_FRAME_WIDTH, cw);
        acidcam::cap.set(cv::CAP_PROP_FRAME_HEIGHT, ch);
        acidcam::cap.set(cv::CAP_PROP_FPS, fps);
        cw = acidcam::cap.get(cv::CAP_PROP_FRAME_WIDTH);
        ch = acidcam::cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        fps = acidcam::cap.get(cv::CAP_PROP_FPS);
    } else {
        acidcam::cap.open(filename);
        if(!acidcam::cap.isOpened()) {
            std::cerr << "Error could not open file: " << filename << "\n";
            exit(EXIT_FAILURE);
        }
        cw = acidcam::cap.get(cv::CAP_PROP_FRAME_WIDTH);
        ch = acidcam::cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        fps = acidcam::cap.get(cv::CAP_PROP_FPS);
    }
    std::cout << "Actual " << ((filename.length()==0) ? "Camera" : "File") << " Resolution: " << cw << "x" << ch << "p" << fps << " \n";
    main_window.create(full, "acidcamGL", w, h);
    std::cout << "acidcamGL: " << version_info << "\n";
    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    glfwSetKeyCallback(main_window.win(), key_callback);
    glfwSetWindowSizeCallback(main_window.win(), window_size_callback);
    main_window.setDebug(debug_val);
    main_window.loadShaders(shader_path);
    main_window.setShader(0);
    main_window.setPrintText(print_text);
    main_window.loop();
    glfwTerminate();
    return EXIT_SUCCESS;
}
