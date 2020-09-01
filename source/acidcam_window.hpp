#include"gl_window.hpp"
#include"gl_shader.hpp"
#include"glm/glm.hpp"
#include"glm/gtc/type_ptr.hpp"
#include"glm/gtc/matrix_transform.hpp"
#ifdef _WIN32
#include"getopt.h"
#include"ac.h"
#else
#include"acidcam/ac.h"
#include<unistd.h>
#endif
#include<vector>
#include<sstream>
#include<string>
#include<fstream>
#include<iomanip>
#include<iostream>
#include<chrono>
#include<algorithm>
#include"keymap.hpp"
#include"ipc_client.hpp"
#define version_info "v1.0.003"
#ifdef SYPHON_SERVER
#include"syphon.h"
#endif
#include<unordered_map>
#ifdef SYPHON_SERVER
extern void ScreenGrabRect(int x, int y, int w, int h, cv::Mat &frame);
#endif

namespace acidcam {

extern cv::VideoCapture cap;
extern int redir;
extern int syphon_enabled;

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
    bool repeat;
    int color_map;
    int blend_index;
    int sx, sy;
    bool video_mode;
    int fps;
    int screen_x, screen_y;
    bool playback_mode;
    bool playback_sort;
    int p_timeout;
    bool rand_timeout;
    bool paused;
    bool stereo_;
public:
    
    AcidCam_Window() = default;
    AcidCam_Window(const AcidCam_Window &) = delete;
    AcidCam_Window &operator=(const AcidCam_Window &) = delete;
    
    void setVideoMode(bool b, int f) {
        video_mode = b;
        fps = f;
    }
    
    virtual void init() override {
        stereo_ = false;
        paused = false;
        rand_timeout = false;
        video_mode = false;
        p_timeout = 1;
        blend_index = 0;
        color_map = -1;
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
        repeat = false;
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
        playback_mode = false;
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
            return;
        } else {
            cap.read(frame);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
            sx = frame.cols;
            sy = frame.rows;
        }
    }
    
    int index;
    
    void setShader(int index) {
        program = shaders[index];
        if(debug) {
            std::cout << "acidcam: Shader Program Loaded: " << program.name() << "\n";
        }
    }
    
    void sortPlaylist() {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(var_list.begin(), var_list.end(), std::default_random_engine(seed));
    }
    
    
    void setPlaybackMode(bool v, int timeout = 0, bool  s = false) {
        
        if(p_timeout == 0) {
            std::cout << "acidcam: Error you must set playback timeout with -N\n";
            acidcam::updateError();
        }
        
        playback_mode = v;
        if(s)
            std::sort(var_list.begin(), var_list.end());
        p_timeout = timeout;
    }
    
    GLuint material = 0;
    bool material_on = false;
    float img_cols = 0, img_rows = 0;
    
    void genMaterial(std::string s) {
        glGenTextures(1, &material);
        glBindTexture(GL_TEXTURE_2D, material);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        cv::Mat frame;
        frame = cv::imread(s);
        if(frame.empty()) {
            std::cout << "acidcam: Error could not load texture: " << s << "\n";
            acidcam::updateError();
        }
        
        cv::Mat flipped;
        cv::flip(frame, flipped, 0);
        cv::cvtColor(flipped, frame, cv::COLOR_BGR2RGB);
        
        img_cols = frame.cols;
        img_rows = frame.rows;
        
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
        
        std::cout << "acidcam: Successfully loaded material: " << s << "\n";
        material_on = true;
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
    
    
    int find_solo(const std::string &s) {
        for(int i = 0; i < ac::solo_filter.size(); ++i) {
            if(ac::solo_filter[i] == s)
                return i;
        }
        return -1;
    }
    
    void loadList(const std::string &l, bool s = false) {
        std::fstream file;
        file.open(l, std::ios::in);
        if(!file.is_open()) {
            std::cout << "acidcam: Error could not open playlist: " << l << "\n";
            acidcam::updateError();
        }
        while(!file.eof()) {
            std::string s;
            std::getline(file, s);
            if(file) {
                if(s.length()>0) {
                    int value = find_solo(s);
                    if(value == -1) {
                        std::cout << "acidcam: Error could not find for playlist string: " << s << "\n";
                        //quit();
                        //return;
                    } else
                        var_list.push_back(value);
                }
            }
        }
        file.close();
        std::cout << "acidcam: Playlist loaded [" << var_list.size() << "] items...\n";
        if(s == true) {
            sortPlaylist();
            std::cout << "acidcam: Playlist Shuffled...\n";
        }
    }
    
    bool screen_mode = false;
    
    void enableScreenMode(bool b, int x, int y, int w, int h) {
        screen_mode = b;
        sx = w;
        sy = h;
        screen_x = x;
        screen_y = y;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        cv::Mat frame(h, w, CV_8UC3);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
    }
    
    void takeSnapshot() {
        static int index = 0;
        ++index;
        cv::Mat flipped;
        readFrame(flipped);
        time_t t = time(0);
#ifdef _WIN32
        struct tm mm;
        localtime_s(&mm, &t);
        struct tm* m = &mm;
#else
        struct tm *m;
        m = localtime(&t);
#endif
        std::ostringstream time_stream;
        time_stream << snapshot_prefix << "-" << (m->tm_year + 1900) << "." << std::setw(2) << std::setfill('0') << (m->tm_mon + 1) << "." << std::setw(2) << std::setfill('0') << m->tm_mday << "_" << std::setw(2) << std::setfill('0') << m->tm_hour << "." << std::setw(2) << std::setfill('0') << m->tm_min << "." << std::setw(2) << std::setfill('0') << m->tm_sec <<  "_" << flipped.cols << "x" << flipped.rows << "x" << index << ".png";
        cv::imwrite(time_stream.str(), flipped);
        std::cout << "acidcam: Wrote: " << time_stream.str() << "\n";
        if(redir == 1) {
#ifndef WIN32
            if(redirect != 0) sendString(redirect->getString());
#endif
        }
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
    
    int writer_w = 0, writer_h = 0;
    
    void setWriter(cv::VideoWriter w, int writer_ww, int writer_hh) {
        writer = w;
        writer_set = true;
        writer_w = writer_ww;
        writer_h = writer_hh;
    }
    
    void writeFrame() {
        cv::Mat frame;
        readFrame(frame);
        if(window_width != writer_w && window_height != writer_h) {
            cv::Mat re;
            cv::resize(frame, re, cv::Size(writer_w, writer_h));
            writer.write(re);
        } else
            writer.write(frame);
    }
    
    void setStereo(bool b) {
        stereo_ = b;
        if(b == true)
            std::cout << "acidcam: Stereo mode enabled...\n";
    }
    
    virtual void update(double timeval) override {
        if(paused)
            return;
        std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        program.useProgram();
        mv_loc = glGetUniformLocation(program.id(), "mv_matrix");
        proj_loc = glGetUniformLocation(program.id(),"proj_matrix");
        GLuint samp = glGetUniformLocation(program.id(),"samp");
        GLuint mat_samp = glGetUniformLocation(program.id(),"mat_samp");
        GLuint calpha_r = glGetUniformLocation(program.id(),"value_alpha_r");
        GLuint calpha_g = glGetUniformLocation(program.id(),"value_alpha_g");
        GLuint calpha_b = glGetUniformLocation(program.id(),"value_alpha_b");
        GLuint c_index = glGetUniformLocation(program.id(),"index_value");
        GLuint c_tf = glGetUniformLocation(program.id(),"time_f");
        GLuint alpha_pos = glGetUniformLocation(program.id(), "alpha_value");
        GLuint optx_pos = glGetUniformLocation(program.id(), "optx");
        GLuint rand_pos = glGetUniformLocation(program.id(), "random_var");
        GLuint restore_blackx = glGetUniformLocation(program.id(), "restore_black");
        GLuint inc_value_pos = glGetUniformLocation(program.id(), "inc_value");
        GLuint inc_value_posx = glGetUniformLocation(program.id(), "inc_valuex");
        
        
        GLuint material_size;
        material_size = glGetUniformLocation(program.id(), "mat_size");
        
        v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        m_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
        
        mv_mat = v_mat * m_mat;
        
#ifdef SYPHON_SERVER
        if(syphon_enabled) {
            syphon_bind(window_width, window_height);
        }
#endif
        
        glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0,0);
        glEnableVertexAttribArray(0);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(1);
        
        cv::Mat frame;
        
        
        if(screen_mode == false) {
            if(!cap.read(frame)) {
                if(repeat == true && repeat_filename.length()>0) {
                    cap.open(repeat_filename);
                    if(cap.isOpened()) {
                        cap.read(frame);
                        std::cout << "acidcam: video loop...\n";
                    } else {
                        std::cout << "acidcam: Capture device closed exiting...\n";
                        quit();
                        return;
                    }
                } else {
                    std::cout << "acidcam: Capture device closed exiting...\n";
                    quit();
                    return;
                }
            }
        } else {
#ifdef SYPHON_SERVER
            cv::Mat cvMat, temp_frame;
            ScreenGrabRect(screen_x, screen_y, window_width, window_height, cvMat);
            cv::cvtColor(cvMat, temp_frame, cv::COLOR_RGBA2BGR);
            cv::resize(temp_frame, cvMat, cv::Size(sx, sy));
            frame = cvMat;
#endif
        }
        
        if(playback_mode && list_enabled) {
            static int playback_index = 0;
            static int frame_time = 0;
            static int current_timeout = p_timeout;
            ++frame_time;
            if(p_timeout == 0 || frame_time > current_timeout) {
                ++playback_index;
                if(rand_timeout)
                    current_timeout = 1+rand()%p_timeout;
            }
            if(frame_time > p_timeout)
                frame_time = 0;
            if(playback_index > var_list.size()) {
                playback_index = 0;
                sortPlaylist();
            }
            index = var_list[playback_index];
        }
    
        if(shader_index == 0 || ac_on == true) {
            if(index >= 0 && index < ac::solo_filter.size()) {
                cv::Mat orig;
                
                if(blend_index > 0) {
                    orig = frame.clone();
                }
                
                CallCustom(ac::solo_filter[index], frame);
                
                if(blend_index > 0) {
                    cv::Mat copyf;
                    double per = 1.0/(blend_index/10);
                    ac::AlphaBlendDouble(frame, orig, copyf, per, 1.0-per);
                    frame = copyf.clone();
                }
                
                if(color_map != -1) {
                    cv::Mat output_f1 = frame.clone();
                    cv::applyColorMap(output_f1, frame, color_map);
                }
                
                if(restore_black == true) {
                    ac::CallFilter("RestoreBlack", frame);
                    if(color_map != -1) {
                        cv::Mat output_f1 = frame.clone();
                        cv::applyColorMap(output_f1, frame, color_map);
                    }
                }
                if(print_text == true) {
                    std::ostringstream stream;
                    stream << ac::solo_filter[index];
                    cv::putText(frame, stream.str(),cv::Point(40, 40),cv::FONT_HERSHEY_DUPLEX,1.0,CV_RGB(255, 255, 255), 2);
                }
            }
        }
        
        cv::flip(frame, frame, 0);
        if(stereo_)
            ac::Stereo(frame);
        
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
        
        if(material_on) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, material);
        }
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
            alpha += 0.1f;
            if(alpha >= 6.0)
                idir = false;
        } else {
            alpha -= 0.1f;
            if(alpha <= 1.0f) {
                idir = true;
            }
            
        }
        random_var = glm::vec4(rand()%255, rand()%255, rand()%255, rand()%255);
        
        static glm::vec4 inc_value(rand()%255, rand()%255, rand()%255, 1);
        
        static int inc_dir = 1;
        
        if(inc_dir == 1) {
            for(int i = 0; i < 3; ++i) {
                ++inc_value[i];
                if(inc_value[i] >= 255)
                    inc_dir = 0;
            }
        } else {
            for(int i = 0; i < 3; ++i) {
                --inc_value[i];
                if(inc_value[i] <= 1) {
                    inc_dir = 1;
                    inc_value = glm::vec4(rand()%255, rand()%255, rand()%255, 1);
                }
            }
        }
        
        static glm::vec4 inc_valuex(rand()%255, rand()%255, rand()%255, 1);
        
        static int inc_dirx = 1;
        
        if(inc_dirx == 1) {
            for(int i = 0; i < 3; ++i) {
                ++inc_valuex[i];
                if(inc_valuex[i] >= 255)
                    inc_dirx = 0;
            }
        } else {
            for(int i = 0; i < 3; ++i) {
                --inc_valuex[i];
                if(inc_valuex[i] <= 1) {
                    inc_dirx = 1;
                }
            }
        }
        
        glUniform1i(samp, 0);
        glUniform1i(mat_samp, 1);
        glUniform1f(c_index, (float)index);
        glUniform1f(c_tf, timeval);
        glUniform4fv(inc_value_pos, 1, glm::value_ptr(inc_value));
        glUniform4fv(inc_value_posx, 1, glm::value_ptr(inc_valuex));
        
        glUniform1f(calpha_r, color_alpha_r);
        glUniform1f(calpha_g, color_alpha_g);
        glUniform1f(calpha_b, color_alpha_b);
        glUniform1f(alpha_pos, alpha);
        glUniform4fv(optx_pos, 1, glm::value_ptr(optx));
        glUniform4fv(rand_pos, 1, glm::value_ptr(random_var));
        glUniform1f(restore_blackx, ((restore_black == true) ? 1.0 : 0.0));
        GLint loc = glGetUniformLocation(program.id(), "iResolution");
        glUniform2f(loc, width, height);
        glUniform2f(material_size, img_cols, img_rows);
        
        
        glDrawArrays(GL_TRIANGLES,0,6);
        
        
#ifdef SYPHON_SERVER
        int tex = syphon_pushTexture(texture);
        glBindTexture(GL_TEXTURE_RECTANGLE_EXT, tex);
        glDrawArrays(GL_TRIANGLES, 0, 6);
#endif
        
        if(take_snapshot == true) {
            takeSnapshot();
            take_snapshot = false;
        }
        if(writer_set == true)
            writeFrame();
        
        if(video_mode && fps != 0) {
            std::chrono::time_point<std::chrono::system_clock> nowx =
            std::chrono::system_clock::now();
            auto m = std::chrono::duration_cast<std::chrono::milliseconds>(nowx-now).count();
            int fps_mil = 1000/fps;
            if(m < fps_mil)
                std::this_thread::sleep_for(std::chrono::milliseconds(fps_mil-m-1));
        }
    }
    
    void setDebug(bool d) {
        debug = d;
    }
    
    std::string repeat_filename;
    
    void setRepeat(std::string fn, bool r) {
        repeat = r;
        repeat_filename = fn;
    }
    
    void setColorMap(int map) {
        color_map = map;
        if(map != -1) {
            std::cout << "acidccam: Set color map: " << map << "\n";
        }
    }
    
    std::string input_string;
    
    void typeKey(unsigned int key) {
        if(key >= '0' && key <= '9')
            input_string += static_cast<char>(key);
    }
    
    void setFilterIndex(const int &i) {
        index = i;
    }
    
    void keypress(int key, int scancode, int action, int mode) {
        if(key == GLFW_KEY_ESCAPE) {
            quit();
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
                case GLFW_KEY_COMMA:
                    
                    if(color_map == 0) {
                        std::cout << "acidcam: Color map disabled...\n";
                        color_map = -1;
                        return;
                    }
                    
                    if(color_map > 0) {
                        color_map--;
                        
                        std::cout << "acidcam: Color Map Decreased to: " << color_map << "\n";
                    }
                    break;
                case GLFW_KEY_PERIOD:
                    if(color_map < 19) {
                        color_map++;
                        std::cout << "acidcam: Color map increased to: " << color_map << "...\n";
                    }
                    break;
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
                case GLFW_KEY_ENTER:
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
                                std::cout << "acidcam: Filter Index: " << index << " - " << ac::solo_filter[index] << "\n";
                            }
                        }
                    } else {
                        if(var_index > 0)
                            var_index -= 1;
                        
                        if(var_list[var_index] > 0 && var_list[var_index] < ac::solo_filter.size()) {
                            index = var_list[var_index];
                            if(debug) {
                                std::cout << "acidcam: Filter Index: " << index << " - " << ac::solo_filter[index] << "\n";
                            }
                        }
                    }
                    break;
                case GLFW_KEY_RIGHT:
                    if(list_enabled == false) {
                        if(index < ac::solo_filter.size()) {
                            ++index;
                            if(debug) {
                                std::cout << "acidcam: Filter Index: " << index << " - " << ac::solo_filter[index] << "\n";
                            }
                        }
                    } else {
                        if(var_index < var_list.size())
                            var_index += 1;
                        if(var_list[var_index] > 0 && var_list[var_index] < ac::solo_filter.size()) {
                            index = var_list[var_index];
                            if(debug) {
                                std::cout << "acidcam: Filter Index: " << index << " - " << ac::solo_filter[index] << "\n";
                            }
                        }
                    }
                    break;
                case GLFW_KEY_M:
                    rand_timeout = !rand_timeout;
                    if(rand_timeout)
                        std::cout << "acidcam: Rand playlist timeout enabled\n";
                    else
                        std::cout << "acidcam: Rand playlist timeout disabled\n";
                    break;
                case GLFW_KEY_MINUS:
                    if(blend_index > 0) {
                        blend_index -= 10;
                        std::cout << "acidcam: Blend Index Set to: " << blend_index << ".\n";
                    }
                    break;
                case GLFW_KEY_EQUAL:
                    if(blend_index < 100) {
                        blend_index += 10;
                        std::cout << "acidcam: Blend Index Set to: " << blend_index << ".\n";
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
                case GLFW_KEY_H:
                    std::cout << "acidcam: Playlist shuffled...\n";
                    sortPlaylist();
                    break;
                case GLFW_KEY_G:
                    
                    if(paused) {
                        std::cout << "acidcam: program unpaused\n";
                        paused = false;
                    } else {
                        std::cout << "acidcam: program paused...\n";
                        paused = true;
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
        
        if(redir == 1) {
#ifndef WIN32
            if(redirect != 0) sendString(redirect->getString());
#endif
        }
    }
    
    void resize(int newWidth, int newHeight) {
        aspect = (float)newWidth / (float)newHeight;
        glViewport(0, 0, newWidth, newHeight);
        p_mat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
        window_width = newWidth;
        window_height = newHeight;
#ifdef SYPHON_SERVER
        if(syphon_enabled) {
            syphon_size(width, height);
        }
#endif
    }
    
    //std:::unordered_map<std::string, std::vector<std::string>> custom_filters;
    
    using List = std::vector<std::string>;
    
    std::unordered_map<std::string, List> custom_filters;
    
    void CallCustom(std::string index, cv::Mat &frame) {
        std::string val = index;
        if(custom_filters.find(val) != custom_filters.end()) {
            for(int i = 0; i < custom_filters[val].size(); ++i) {
                ac::CallFilter(custom_filters[val].at(i),frame);
            }
        } else {
            ac::CallFilter(index, frame);
        }
    }
    
    void loadCustom(std::string &c) {
        
        std::string path = c + "/index.txt";
        std::fstream file;
        file.open(path, std::ios::in);
        if(!file.is_open()) {
            std::cout << "acidcam: Couldn't load custom files: " << path << "\n";
            acidcam::updateError();
        }
        while(!file.eof()) {
            std::string n;
            std::getline(file, n);
            if(file) {
                std::fstream custom;
                custom.open(c + "/" + n, std::ios::in);
                if(!custom.is_open()) {
                    std::cout << "acidcam: Error loading custom file: " << n << "\n";
                    acidcam::updateError();
                }
                custom_filters[n] = std::vector<std::string>();
                while(!custom.eof()) {
                    std::string v;
                    std::getline(custom, v);
                    if(custom) {
                        custom_filters[n].push_back(v);
                    }
                }
                std::cout << "acidcam: Load Custom: " << n << "\n";
                ac::solo_filter.push_back(n);
                custom.close();
            }
        }
    }
    
    void loadShaders(const std::string &text) {
        std::ostringstream stream;
        stream << text << "/" << "index.txt";
        std::fstream file;
        file.open(stream.str(), std::ios::in) ;
        if(!file.is_open()) {
            std::cout << "acidcam: Error could not open: " << stream.str() << "\n";
            acidcam::updateError();
        }
        
        int index = 0;
        
        while(!file.eof()) {
            
            if(acidcam::redir == 1) {
#ifndef _WIN32
                if(redirect != 0) {
                    std::string text = redirect->getString();
                    sendString(text);
                }
#endif
            }
            
            std::string s;
            std::getline(file, s);
            if(file && s.length() > 0) {
                if(material_on == false && s.find("material") != std::string::npos)
                    continue;
                std::ostringstream fs1,fs2;
                fs1 << text << "/" << s;
                fs2 << text << "/vertex.glsl";
                ShaderProgram p;
                std::cout << "acidcam: Compiling Shader " << index << " [" << s << "] ";
                if(p.loadProgram(fs2.str(), fs1.str())==false) {
                    std::cout << "Error could not load: " << fs1.str() << "\n";
                    acidcam::updateError();
                }
                std::cout << "\n";
                p.setName(s.substr(0, s.rfind(".")));
                shaders.push_back(p);
                ++index;
            }
        }
        file.close();
        std::cout << "acidcam: Loaded " << shaders.size() << " Shaders...\n";
    }
};
}
