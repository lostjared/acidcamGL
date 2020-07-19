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
#include<iomanip>
#include<iostream>
#include"keymap.hpp"

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
        cv::VideoWriter writer;
        bool writer_set = false;
        int shader_index;
        bool print_text;
        bool debug;
        float alpha;
        bool ac_on;
        glm::vec4 optx, random_var;
        KeyMap mapped_keys;
        float movement_rate;
        bool take_snapshot;
        std::string snapshot_prefix;
        bool restore_black;
        bool list_enabled;
        std::vector<int> var_list;
        int var_index;
    public:
        
        AcidCam_Window() = default;
        AcidCam_Window(const AcidCam_Window &) = delete;
        AcidCam_Window &operator=(const AcidCam_Window &) = delete;
        
        virtual void init() override {
            var_index = 0;
            list_enabled = false;
            restore_black = false;
            snapshot_prefix="AcidCamGL_Snapshot";
            take_snapshot = false;
            optx = glm::vec4(0.5,0.5,0.5,0.5);
            index = 0;
            movement_rate = 0.07f;
            ac_on = false;
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
                std::cerr << "acidcam: Error opening camera/file..\n";
                exit(EXIT_FAILURE);
            }
            cap.read(frame);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
        }
        
        int index;
        
        void setShader(int index) {
            program = shaders[index];
            if(debug) {
                std::cout << "acidcam: Shader Program Loaded: " << program.name() << "\n";
            }
        }
        
        void setPrintText(bool b) {
            print_text = b;
        }
        
        void setPrefix(const std::string &s) {
            snapshot_prefix = s;
        }
        
        void setRestoreBlack(bool &b) {
            restore_black = b;
        }
        
        void loadKeys(const std::string &n) {
            mapped_keys.load(n);
        }
        
        void loadList(const std::string &l) {
            std::fstream file;
            file.open(l, std::ios::in);
            while(!file.eof()) {
                std::string s;
                std::getline(file, s);
                if(file) {
                    if(s.length()>0) {
                        int value = atoi(s.c_str());
                        var_list.push_back(value);
                    }
                }
            }
            file.close();
        }
        
        void takeSnapshot() {
            static int index = 0;
            ++index;
            cv::Mat flipped;
            readFrame(flipped);
            time_t t = time(0);
            struct tm *m;
            m = localtime(&t);
            std::ostringstream time_stream;
            time_stream << snapshot_prefix << "-" << (m->tm_year + 1900) << "." << std::setw(2) << std::setfill('0') << (m->tm_mon + 1) << "." << std::setw(2) << std::setfill('0') << m->tm_mday << "_" << std::setw(2) << std::setfill('0') << m->tm_hour << "." << std::setw(2) << std::setfill('0') << m->tm_min << "." << std::setw(2) << std::setfill('0') << m->tm_sec <<  "_" << flipped.cols << "x" << flipped.rows << "x" << index << ".png";
            cv::imwrite(time_stream.str(), flipped);
            std::cout << "acidcam: Wrote: " << time_stream.str() << "\n";
        }
        
        void readFrame(cv::Mat &frame) {
            cv::Mat img;
            img.create(window_height, window_width,CV_8UC3);
            glPixelStorei(GL_PACK_ALIGNMENT, (img.step & 3) ? 1 : 4);
            glPixelStorei(GL_PACK_ROW_LENGTH, (int)img.step/img.elemSize());
            glReadPixels(0, 0, img.cols, img.rows, GL_RGB, GL_UNSIGNED_BYTE, img.data);
            cv::Mat flipped;
            cv::flip(img, flipped, 0);
            cv::cvtColor(flipped, frame, cv::COLOR_RGB2BGR);
        }
        
        void setWriter(cv::VideoWriter w) {
            writer = w;
            writer_set = true;
        }
        
        void writeFrame() {
            cv::Mat frame;
            readFrame(frame);
            writer.write(frame);
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
            GLuint optx_pos = glGetUniformLocation(program.id(), "optx");
            GLuint rand_pos = glGetUniformLocation(program.id(), "random_var");
            
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
                std::cout << "acidcam: Capture device closed exiting...\n";
                exit(EXIT_SUCCESS);
            }
            if(shader_index == 0 || ac_on == true) {
                if(index >= 0 && index < ac::solo_filter.size()-3) {
                    ac::CallFilter(ac::solo_filter[index], frame);
                    if(restore_black == true) {
                        ac::CallFilter("RestoreBlack", frame);
                    }
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
            static bool idir = true;
            if(idir == true) {
                alpha += 0.01f;
                if(alpha >= 25.0)
                    idir = false;
            } else {
                alpha -= 0.1f;
                if(alpha <= 0.1f)
                    idir = true;
            
            }
            random_var = glm::vec4(rand()%255, rand()%255, rand()%255, rand()%255);
            glUniform1i(samp, 0);
            glUniform1f(c_index, (float)index);
            glUniform1f(c_tf, timeval);
            glUniform1f(calpha_r, color_alpha_r);
            glUniform1f(calpha_g, color_alpha_g);
            glUniform1f(calpha_b, color_alpha_b);
            glUniform1f(alpha_pos, alpha);
            glUniform4fv(optx_pos, 1, glm::value_ptr(optx));
            glUniform4fv(rand_pos, 1, glm::value_ptr(random_var));
            glDrawArrays(GL_TRIANGLES,0,6);
            
            if(take_snapshot == true) {
                takeSnapshot();
                take_snapshot = false;
            }
            if(writer_set == true)
                writeFrame();
        }
        
        void setDebug(bool d) {
            debug = d;
        }
        
        std::string input_string;
        
        void typeKey(unsigned int key) {
            if(key >= '0' && key <= '9')
                input_string += static_cast<char>(key);
        }
        
        void keypress(int key, int scancode, int action, int mode) {
            if(key == GLFW_KEY_ESCAPE) {
                active = false;
                return;
            }
            
            if(action == GLFW_RELEASE) {
                int f = 0, s = 0;
                if(mapped_keys.checkKey(key, f, s)) {
                    index = f;
                    std::cout << "acidcam: Filter: " << ac::solo_filter[index] << "\n";
                    setShader(s);
                    return;
                }
                
                switch(key) {
                    case GLFW_KEY_L:
                        if(var_list.size()>0) {
                            static int offset = 0;
                            list_enabled = !list_enabled;
                            if(list_enabled == true && var_list[var_index] >= 0 && var_list[var_index] < ac::solo_filter.size()) {
                                offset = index;
                                index = var_list[var_index];
                                std::cout << "acidcam: filter mode [list] enabled...\n";
                                
                            } else if(list_enabled == false) {
                                index = offset;
                                std::cout << "acidcam: filter mode [normal] restored...\n";
                            }
                        }
                        break;
                    case GLFW_KEY_N:
                        
                        if(debug)
                            std::cout << "acidcam: filter index set to end of list...\n";
                        if(list_enabled == false)
                            index = ac::solo_filter.size()-3;
                        break;
                    case GLFW_KEY_P:
                        if(debug)
                            std::cout << "acidcam: filter index reset...\n";
                        
                        if(list_enabled == false)
                            index = 0;
                        break;
                    case GLFW_KEY_K:
                        if(index+25 < ac::solo_filter.size()-3)
                            index += 25;
                        break;
                    case GLFW_KEY_J:
                        if(index-25 > 0)
                            index -= 25;
                        break;
                    case GLFW_KEY_Z:
                        take_snapshot = true;
                        break;
                    case GLFW_KEY_F: {
                        int val = atoi(input_string.c_str());
                        if(val >= 0 && val <= ac::solo_filter.size()-3) {
                            index = val;
                            input_string = "";
                            std::cout << "acidcam: Filter: " << ac::solo_filter[index] << "\n";
                        }
                    }
                        break;
                    case GLFW_KEY_S: {
                        int val = atoi(input_string.c_str());
                        if(val >= 0 && val <= shaders.size()-1) {
                            input_string = "";
                            setShader(val);
                        }
                    }
                        break;
                    case GLFW_KEY_C: {
                        input_string = "";
                        break;
                    }
                    case GLFW_KEY_SPACE:
                        ac_on = !ac_on;
                        if(ac_on)
                            std::cout << "acidcam: filters enabled...\n";
                        else
                            std::cout << "acidcam: filters disabled...\n";
                        break;
                    case GLFW_KEY_LEFT:
                        if(list_enabled == false) {
                            if(index > 0) {
                                --index;
                                if(debug) {
                                    std::cout << "acidcam: " << ac::solo_filter[index] << "\n";
                                }
                            }
                        } else {
                            if(var_index > 0)
                                var_index -= 1;
                            
                            if(var_list[var_index] > 0 && var_list[var_index] < ac::solo_filter.size()) {
                                index = var_list[var_index];
                                if(debug) {
                                    std::cout << "acidcam: " << ac::solo_filter[index] << "\n";
                                }
                            }
                        }
                        break;
                    case GLFW_KEY_RIGHT:
                        if(list_enabled == false) {
                            if(index < ac::solo_filter.size()-3) {
                                ++index;
                                if(debug) {
                                    std::cout << "acidcam: " << ac::solo_filter[index] << "\n";
                                }
                            }
                        } else {
                            if(var_index < var_list.size())
                                var_index += 1;
                            if(var_list[var_index] > 0 && var_list[var_index] < ac::solo_filter.size()) {
                                index = var_list[var_index];
                                if(debug) {
                                    std::cout << "acidcam: " << ac::solo_filter[index] << "\n";
                                }
                            }
                        }
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
                    case GLFW_KEY_Q:
                        optx[0] -= movement_rate;
                        break;
                    case GLFW_KEY_W:
                        optx[0] += movement_rate;
                        break;
                    case GLFW_KEY_E:
                        optx[1] -= movement_rate;
                        break;
                    case GLFW_KEY_R:
                        optx[1] += movement_rate;
                        break;
                    case GLFW_KEY_Y:
                        optx[2] -= movement_rate;
                        break;
                    case GLFW_KEY_U:
                        optx[2] += movement_rate;
                        break;
                    case GLFW_KEY_I:
                        optx[3] -= movement_rate;
                        break;
                    case GLFW_KEY_O:
                        optx[3] += movement_rate;
                        break;
                    case GLFW_KEY_T:
                        optx = glm::vec4(0.5, 0.5, 0.5, 0.5);
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
                std::cerr << "acidcam: Error could not open: " << stream.str() << "\n";
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
                    std::cout << "acidcam: Compiling Shader [" << s << "] ";
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
            std::cout << "acidcam: Loaded " << shaders.size() << " Shaders...\n";
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

void character_callback(GLFWwindow* window, unsigned int codepoint) {
    main_window.typeKey(codepoint);
}

void print_help_message() {
    std::cout << "Written by Jared Bruni\narguments:\n-h use h264 output\n-o output mp4 filename\n-k shortcut-key file\n-L playlist of filters\n-b restore black\n-g output debug strings\n-u fps\n-n print filter name\n-e snapshot prefix\n-p shader path\n-f fullscreen\n-d capture device\n-r resolution 1920x1080\n-c Camera resolution 1280x720\n-l list filters\n-v version\n";
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
    std::cout << "acidcamGL: " << version_info << "\n";
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
    std::string key_val;
    std::string snapshot_prefix="AcidCamGL_Snapshot";
    bool restore_black = false;
    std::string list_var;
    std::string output_file;
    bool h264 = false;
    while((opt = getopt(argc, argv, "hbgu:p:i:c:r:d:fhvj:snlk:e:L:o:")) != -1) {
        switch(opt) {
            case 'h':
                h264 = true;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'L':
                list_var = optarg;
                break;
            case 'b':
                restore_black = true;
                break;
            case 'e':
                snapshot_prefix = optarg;
                break;
            case 'k':
                key_val = optarg;
                break;
            case 'l':
                std::cout << "Filters by Index: \n";
                for(int i = 0; i < ac::solo_filter.size(); ++i) {
                    std::cout << i << ": " << ac::solo_filter[i] << "\n";
                }
                exit(EXIT_SUCCESS);
                break;
            case 'g':
                debug_val = true;
                break;
            case 'u':
                fps = atof(optarg);
                break;
            case 'n':
                print_text = true;
                break;
            case 'p':
                shader_path = optarg;
                break;
            case 'i':
                filename = optarg;
                break;
            case 'v':
                print_help_message();
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
                    std::cerr << "acidcam: Invalid format for resolution string...1920x1080 is proper.\n";
                    exit(EXIT_FAILURE);
                }
                std::string left=pos.substr(0, pos.rfind("x"));
                std::string right=pos.substr(pos.rfind("x")+1, pos.length());
                w = atoi(left.c_str());
                h = atoi(right.c_str());
                if(w <= 0 || h <= 0) {
                    std::cerr << "acidcam: Invalid resolution..\n";
                    exit(EXIT_FAILURE);
                }
                std::cout << "acidcam: Setting Window Resolution at: " << w << "x" << h << "\n";
            }
                break;
            case 'c': {
                std::string pos = optarg;
                if(pos.rfind("x") == std::string::npos) {
                    std::cerr << "acidcam: Invalid format for resolution string...1920x1080 is proper.\n";
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
                std::cout << "acidcam: Desired Capture Resolution: " << cw << "x" << ch << "\n";
            }
                break;
            case 'j':
                joy_index = atoi(optarg);
                break;
        }
    }
    if(shader_path.length()==0) {
        std::cerr << "acidcam: Error: must provide path to shaders...\n";
        exit(EXIT_FAILURE);
    }
    
    cv::VideoWriter writer;
    
    if(filename.length()==0) {
        acidcam::cap.open(device);
        if(!acidcam::cap.isOpened()) {
            std::cerr << "acidcam: Could not open capture device...\n";
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
            std::cerr << "acidcam: Error could not open file: " << filename << "\n";
            exit(EXIT_FAILURE);
        }
        cw = acidcam::cap.get(cv::CAP_PROP_FRAME_WIDTH);
        ch = acidcam::cap.get(cv::CAP_PROP_FRAME_HEIGHT);
        fps = acidcam::cap.get(cv::CAP_PROP_FPS);
    }
    
    main_window.create(((output_file.length()>0)?true:false), full, "acidcamGL", w, h);
    std::cout << "acidcam: GL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "acidcam: Actual " << ((filename.length()==0) ? "Camera" : "File") << " Resolution: " << cw << "x" << ch << "p" << fps << " \n";
    glfwSetKeyCallback(main_window.win(), key_callback);
    glfwSetWindowSizeCallback(main_window.win(), window_size_callback);
    glfwSetCharCallback(main_window.win(), character_callback);
    if(list_var.length()>0)
        main_window.loadList(list_var);
    
    main_window.setDebug(debug_val);
    main_window.loadShaders(shader_path);
    main_window.setShader(0);
    main_window.setPrintText(print_text);
    main_window.setPrefix(snapshot_prefix);
    main_window.setRestoreBlack(restore_black);
    if(output_file.length()>0) {
        if(h264)
            writer.open(output_file, cv::VideoWriter::fourcc('a','v','c','1'), fps, cv::Size(w, h), true);
        else
            writer.open(output_file, cv::VideoWriter::fourcc('m','p','4','v'), fps, cv::Size(w, h), true);
        if(!writer.isOpened()) {
            std::cerr << "acidcam: Error opening video writer...\n";
            exit(EXIT_FAILURE);
        }
        std::cout << "acidcam: record " << output_file << " " << w << "x" << h << " " << fps << "\n";
        main_window.setWriter(writer);
    }
    if(key_val.length()>0)
        main_window.loadKeys(key_val);
    std::cout << "acidcam: initialized...\n";
    main_window.loop();
    if(writer.isOpened())
        std::cout << "acidcam: wrote to file [" << output_file << "]\n";
    writer.release();
    std::cout << "acidcam: exited\n";
    glfwTerminate();
    return EXIT_SUCCESS;
}
