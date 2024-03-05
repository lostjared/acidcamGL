#include"gl_window.hpp"
#include"gl_shader.hpp"
#include"autofilter.hpp"
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
#include<cctype>
#include"keymap.hpp"
#include"ipc_client.hpp"
#define version_info "v1.3.2"
#ifdef SYPHON_SERVER
#include"syphon.h"
#endif
#include<unordered_map>
#ifdef SYPHON_SERVER
extern void ScreenGrabRect(int x, int y, int w, int h, cv::Mat &frame);
#endif
#ifdef FOR_LINUX
#include"x11-screen.hpp"
#endif
#include"ffmpeg_write.h"
#ifdef _WIN32
#define SPOUT_SERVER
#endif

#ifdef SPOUT_SERVER
#include"SpoutLibrary.h"
#endif
#include"plugin-program.hpp"
#include"stereo.h"

extern int findFilter(std::string f);

namespace acidcam {
    
    extern cv::VideoCapture cap, cap_second;
    extern int redir;
    extern int syphon_enabled;

#ifdef FOR_LINUX
    extern ScreenShot *screen_shot;
#endif
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
        int stored_position, stored_var_position;
        bool rand_shader;
        bool shader_list_enabled;
        bool stereo_mode = false;
        FILE *fptr;
        std::vector<int> shader_list;
        std::unordered_map<std::string, int> shader_map;
        bool enable_cubeapp = false;
        bool blur_enabled;
        std::string output_format;
#ifdef SPOUT_SERVER
        SPOUTLIBRARY* spoutsender;
#endif
        glm::vec4 inc_value;
        glm::vec4 inc_valuex;
        bool p_run = false;
        AutoFilter af;
        bool af_enabled = false;
        std::string material_file;
    public:
        
        AcidCam_Window() = default;
        AcidCam_Window(const AcidCam_Window &) = delete;
        AcidCam_Window &operator=(const AcidCam_Window &) = delete;
        
        void setOutputFormat(std::string f) {
            output_format = f;
        }
        
        void loadAutoFilter(const std::string &f) {
            if(af.loadFile(f))
                af_enabled = true;
        }
        
        void enableCube(bool value) {
            enable_cubeapp = value;
            if(value)
                std::cout << "acidcam: Cube mode enabled...\n";
        }
        
        StereoCam stereo;
        
        void StereoX(int *id, int w, int h, double fps_) {
            stereo.Load(id);
            stereo.SetSize(w, h, fps_);
            stereo_ = true;
            stereo_mode = true;
        }
        
        
#ifdef SPOUT_SERVER
        bool INIT_TEXTURE(GLuint& texID, unsigned int width, unsigned int height)
        {
            if (texID != 0) glDeleteTextures(1, &texID);
            
            glGenTextures(1, &texID);
            glBindTexture(GL_TEXTURE_2D, texID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glBindTexture(GL_TEXTURE_2D, 0);
            
            return true;
        }
#endif
        
        void setVideoMode(bool b, int f) {
            video_mode = b;
            fps = f;
        }
        
        void setFilePointer(FILE *fptr, int w, int h) {
            this->fptr = fptr;
            writer_w = w;
            writer_h = h;
        }
        
        GLuint senderTexture = 0;
        
        virtual void init() override {
            blur_enabled = false;
            shader_list_enabled = false;
            fptr = 0;
            rand_shader = false;
            stored_position = 0;
            stored_var_position = 0;
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
            inc_value = glm::vec4(rand() % 255, rand() % 255, rand() % 255, 1);
            inc_valuex = glm::vec4(rand() % 255, rand() % 255, rand() % 255, 1);
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
            
            if(enable_cubeapp) {
                init_cube();
                return;
            }
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
#ifdef SPOUT_SERVER
            spoutsender = GetSpout(); // Create a Spout sender object from the SpoutLibary dll
            if (!spoutsender) {
                MessageBoxA(NULL, "Load Spout library failed", "Spout Sender", MB_ICONERROR);
                exit(0);
            }
            senderTexture = 0; // make sure the ID is zero for the first time
            INIT_TEXTURE(senderTexture, window_width, window_height);
#endif
        }
        
        
        int index;
        
        void setShader(int index) {
            program = shaders[index];
            if(debug) {
                std::cout << "acidcam: Shader Program: " << index << " - Loaded: " << program.name() << "\n";
            }
            shader_index = index;
        }
        
        void sortPlaylist() {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(var_list.begin(), var_list.end(), std::default_random_engine(seed));
        }
        
        
        void setPlaybackMode(bool v, int timeout = 0, int beat = 0, int bpm = 0, bool  s = false) {
            
            if(p_timeout == 0) {
                std::cout << "acidcam: Error you must set playback timeout with -N\n";
                acidcam::updateError();
            }
            playback_mode = v;
            if(s)
                std::sort(var_list.begin(), var_list.end());
            if(bpm != 0 && beat != 0) {
                p_timeout = beat;
                std::cout <<"acidcam: Beats per minute: " << bpm << " frames per beat: " << p_timeout << "\n";
            }
            else
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
            auto lowerString = [](const std::string &sx) -> std::string {
                std::string val;
                for(int i = 0; i < sx.length(); ++i) {
                    val += tolower(sx[i]);
                }
                return val;
            };
            std::string sx = lowerString(s);
            if(sx.find(".mp4") != std::string::npos || sx.find(".mkv") != std::string::npos || sx.find(".avi") != std::string::npos || sx.find(".m4v") != std::string::npos || sx.find(".mov") != std::string::npos) {
                if(!cap_second.open(s)) {
                    std::cout << "acidcam: Error could not open secondary material video...\n";
                    acidcam::updateError();
                    exit(EXIT_FAILURE);
                } else {
                    std::cout << "acidcam: Opened secondary video [" << s << "]\n";
                    material_file = s;
                }
                if(!cap_second.read(frame)) {
                    std::cout << "acidcam: Could not read frame from video.\n";
                    acidcam::updateError();
                    exit(EXIT_FAILURE);
                }
            } else {
                frame = cv::imread(s);
            }
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
            time_stream << snapshot_prefix << "-" << (m->tm_year + 1900) << "." << std::setw(2) << std::setfill('0') << (m->tm_mon + 1) << "." << std::setw(2) << std::setfill('0') << m->tm_mday << "_" << std::setw(2) << std::setfill('0') << m->tm_hour << "." << std::setw(2) << std::setfill('0') << m->tm_min << "." << std::setw(2) << std::setfill('0') << m->tm_sec <<  "_" << flipped.cols << "x" << flipped.rows << "x" << index << "." << output_format;
            cv::imwrite(time_stream.str(), flipped);
            std::cout << "acidcam: Wrote: " << time_stream.str() << "\n";
            if(redir == 1) {
#if defined(__APPLE__) || defined(__linux__)
                if(redirect != 0) sendString(redirect->getString());
#endif
            }
        }
        
        
        void readFrame(cv::Mat &frame) {
            cv::Mat img;
            int width, height;
            glfwGetFramebufferSize(win(), &width, &height);
            img.create(height, width,CV_8UC3);
            GLsizei nrChannels = 3;
            GLsizei stride = nrChannels * width;
            stride += (stride % 4) ? (4 - stride % 4) : 0;
            GLsizei bufferSize = stride * height;
            std::vector<char> buffer(bufferSize);
            glPixelStorei(GL_PACK_ALIGNMENT, 4);
            glReadBuffer(GL_FRONT);
            glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, img.data);
            cv::Mat flipped;
            cv::flip(img, flipped, 0);
            cv::Mat frame1;
            cv::cvtColor(flipped, frame1, cv::COLOR_RGB2BGR);
            if(window_width != width || window_height != height) {
               cv::resize(frame1,frame, cv::Size(window_width, window_height));
            } else
                frame = frame1;
        }
        
        int writer_w = 0, writer_h = 0;
        
        void setWriter(cv::VideoWriter w, int writer_ww, int writer_hh) {
            writer = w;
            writer_set = true;
            writer_w = writer_ww;
            writer_h = writer_hh;
        }
        
        void write(cv::Mat &frame) {
            if(frame.empty()) return;
            if(fptr != 0) {
                cv::Mat reimage;
                if(frame.size() != cv::Size(writer_w, writer_h)) {
                    cv::resize(frame, reimage, cv::Size(writer_w, writer_h));
                    write_ffmpeg(fptr, reimage);
                } else
                    write_ffmpeg(fptr, frame);
            } else {
                if(frame.size() != cv::Size(writer_w,writer_h)) {
                    cv::Mat re;
                    cv::resize(frame, re, cv::Size(writer_w, writer_h));
                    writer.write(re);
                }
                else
                    writer.write(frame);
            }
#if defined(__APPLE__) || defined(__linux__)
            if(redirect != 0) {
                static int frame = 1;
                int total = cap.get(cv::CAP_PROP_FRAME_COUNT);
                if((frame%fps*20)==0 && fps > 0) {
                    std::ostringstream stream;
                    stream << "acidcam: wrote frame: " << frame << "/" << total << "/" << (frame/fps) << " seconds...\n";
                    sendString(stream.str());
                }
                ++frame;
            }
#endif
        }
        
        void writeFrame() {
            static bool skip = true;
            if(skip == true) {
                skip = false;
                return;
            }
            cv::Mat frame;
            readFrame(frame);
            write(frame);
        }
        
        void setStereo(bool b) {
            stereo_mode = b;
            if(b == true)
                std::cout << "acidcam: Stereo mode enabled...\n";
        }
        
        bool bInitialized = false;
        
        virtual void update(double timeval) override {
            if (paused)
                return;
            
            if(enable_cubeapp) {
                update_cube(timeval);
                return;
            }
            
            if(af_enabled && af.size()>0) {
                AutoFilterIndex index{af.current()};
                int sh = shader_map[index.shader];
                program = shaders[sh];
                int i = findFilter(index.filter);
                if(i == -1) {
                    std::cout << "acidcam: Could not find filter: " << index.filter << "\n";
                    exit(EXIT_FAILURE);
                } else {
                    setFilterIndex(i);
                    if(!af.increment()) {
                        af.next();
                        std::cout << "acidcam: [AutoFilter] - " << af.current() << "\n";
                    }
                }
            }
                       
            std::chrono::time_point<std::chrono::system_clock> now =
            std::chrono::system_clock::now();
            
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClearDepth(1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            if (rand_shader == true)
                program = shaders[rand() % (shaders.size() - 1)];
            
            
            program.useProgram();
            mv_loc = glGetUniformLocation(program.id(), "mv_matrix");
            proj_loc = glGetUniformLocation(program.id(), "proj_matrix");
            GLuint samp = glGetUniformLocation(program.id(), "samp");
            GLuint mat_samp = glGetUniformLocation(program.id(), "mat_samp");
            GLuint calpha_r = glGetUniformLocation(program.id(), "value_alpha_r");
            GLuint calpha_g = glGetUniformLocation(program.id(), "value_alpha_g");
            GLuint calpha_b = glGetUniformLocation(program.id(), "value_alpha_b");
            GLuint c_index = glGetUniformLocation(program.id(), "index_value");
            GLuint c_tf = glGetUniformLocation(program.id(), "time_f");
            GLuint alpha_pos = glGetUniformLocation(program.id(), "alpha_value");
            GLuint optx_pos = glGetUniformLocation(program.id(), "optx");
            GLuint rand_pos = glGetUniformLocation(program.id(), "random_var");
            GLuint restore_blackx = glGetUniformLocation(program.id(), "restore_black");
            GLuint inc_value_pos = glGetUniformLocation(program.id(), "inc_value");
            GLuint inc_value_posx = glGetUniformLocation(program.id(), "inc_valuex");
            
            
            GLuint material_size;
            material_size = glGetUniformLocation(program.id(), "mat_size");
            
            v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
            m_mat = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
            
            mv_mat = v_mat * m_mat;
            
#ifdef SYPHON_SERVER
            if (syphon_enabled) {
                syphon_bind(window_width, window_height);
            }
#endif
            
            glUniformMatrix4fv(mv_loc, 1, GL_FALSE, glm::value_ptr(mv_mat));
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(p_mat));
            
            glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);
            
            glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);
            
            cv::Mat frame;
            
            
            if (screen_mode == false) {
                if(stereo_ == false) {
                    if (!cap.read(frame)) {
                        if (repeat == true && repeat_filename.length() > 0) {
                            cap.open(repeat_filename);
                            if (cap.isOpened()) {
                                cap.read(frame);
                                std::cout << "acidcam: video loop...\n";
                            }
                            else {
                                std::cout << "acidcam: Capture device closed exiting...\n";
                                quit();
                                return;
                            }
                        }
                        else {
                            std::cout << "acidcam: Capture device closed exiting...\n";
                            quit();
                            return;
                        }
                    }
                }
                
            }
            else {
#ifdef SYPHON_SERVER
                cv::Mat cvMat, temp_frame;
                ScreenGrabRect(screen_x, screen_y, window_width, window_height, cvMat);
                cv::cvtColor(cvMat, temp_frame, cv::COLOR_RGBA2BGR);
                cv::resize(temp_frame, cvMat, cv::Size(sx, sy));
                frame = cvMat;
#endif
#ifdef FOR_LINUX
                if(screen_shot == nullptr) {
                    screen_shot = new ScreenShot();
                }
                cv::Mat cvMat, temp_frame;
                (*screen_shot)(cvMat, screen_x, screen_y, window_width, window_height);
                cv::cvtColor(cvMat, temp_frame, cv::COLOR_RGBA2BGR);
                cv::resize(temp_frame, cvMat, cv::Size(sx, sy));
                frame = cvMat;
#endif
            }
            
            if (playback_mode && list_enabled) {
                static int playback_index = 0;
                static int frame_time = 0;
                static int current_timeout = p_timeout;
                ++frame_time;
                if (p_timeout == 0 || frame_time > current_timeout) {
                    ++playback_index;
                    if (rand_timeout && p_timeout)
                        current_timeout = 1 + rand() % p_timeout;
                }
                if (frame_time > p_timeout)
                    frame_time = 0;
                if (playback_index > var_list.size()) {
                    playback_index = 0;
                    sortPlaylist();
                }
                index = var_list[playback_index];
            }
            
            if (video_mode == false && stereo_) {
                frame.create(cv::Size(window_width, window_height),CV_8UC3);
                stereo.Render(frame);
                cv::flip(frame, frame, 0);
            } else if(video_mode == true && stereo_mode) {
                ac::Stereo(frame);
            }
            
            if (shader_index == 0 || ac_on == true) {
                if (index >= 0 && index < ac::solo_filter.size()) {
                    cv::Mat orig;
                    
                    if (blend_index > 0) {
                        orig = frame.clone();
                    }
                    
                    CallCustom(ac::solo_filter[index], frame);

                    if (blend_index > 0 && blend_index / 10 > 0) {
                        cv::Mat copyf;
                        double per = 1.0 / (blend_index / 10);
                        ac::AlphaBlendDouble(frame, orig, copyf, per, 1.0 - per);
                        frame = copyf.clone();
                    }
                    
                    if (color_map != -1) {
                        cv::Mat output_f1 = frame.clone();
                        cv::applyColorMap(output_f1, frame, color_map);
                    }
                    
                    if (restore_black == true) {
                        ac::CallFilter("RestoreBlack", frame);
                        if (color_map != -1) {
                            cv::Mat output_f1 = frame.clone();
                            cv::applyColorMap(output_f1, frame, color_map);
                        }
                    }
                    if (print_text == true) {
                        std::ostringstream stream;
                        stream << ac::solo_filter[index];
                        cv::putText(frame, stream.str(), cv::Point(40, 40), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255), 2);
                    }
                }
            } else {
                if(stereo_ == false)
                    cv::flip(frame, frame, 0);
            }
           
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
            
            if (material_on) {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, material);
                cv::Mat frame1;
                if(cap_second.isOpened()) {
                    if(!cap_second.read(frame1)) {
                        cap_second.open(material_file);
                        if(!cap_second.isOpened()) {
                            std::cout << "acidcam: Error opening video: " << material_file << "\n";
                            acidcam::updateError();
                        }
                        if(!cap_second.read(frame1)) {
                            std::cout << "acidcam: Error reading frame..\n";
                            acidcam::updateError();
                            exit(EXIT_FAILURE);
                        }
                    }
                    cv::flip(frame1, frame1, 0);
                    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame1.cols, frame1.rows, GL_BGR, GL_UNSIGNED_BYTE, frame1.ptr());
                }
            }
            color_alpha_r += rand() % 100 * 0.01f;
            color_alpha_g += rand() % 100 * 0.01f;
            color_alpha_b += rand() % 100 * 0.01f;
            
            if (color_alpha_r > 1.5f)
                color_alpha_r = 0.1f;
            if (color_alpha_g > 1.5f)
                color_alpha_g = 0.1f;
            if (color_alpha_b > 1.5f)
                color_alpha_b = 0.1f;
            static bool idir = true;
            if (idir == true) {
                alpha += 0.1f;
                if (alpha >= 6.0)
                    idir = false;
            }
            else {
                alpha -= 0.1f;
                if (alpha <= 1.0f) {
                    idir = true;
                }
                
            }
            random_var = glm::vec4(rand() % 255, rand() % 255, rand() % 255, rand() % 255);
            
            
            static int inc_dir = 1;
            
            if (inc_dir == 1) {
                for (int i = 0; i < 3; ++i) {
                    ++inc_value[i];
                    if (inc_value[i] >= 255)
                        inc_dir = 0;
                }
            }
            else {
                for (int i = 0; i < 3; ++i) {
                    --inc_value[i];
                    if (inc_value[i] <= 1) {
                        inc_dir = 1;
                        inc_value = glm::vec4(rand() % 255, rand() % 255, rand() % 255, 1);
                    }
                }
            }
            
            
            static int inc_dirx = 1;
            
            if (inc_dirx == 1) {
                for (int i = 0; i < 3; ++i) {
                    ++inc_valuex[i];
                    if (inc_valuex[i] >= 255)
                        inc_dirx = 0;
                }
            }
            else {
                for (int i = 0; i < 3; ++i) {
                    --inc_valuex[i];
                    if (inc_valuex[i] <= 1) {
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
            
            glDrawArrays(GL_TRIANGLES, 0, 6);
#ifdef SYPHON_SERVER
            int tex = syphon_pushTexture(texture);
            glBindTexture(GL_TEXTURE_RECTANGLE_EXT, tex);
            glDrawArrays(GL_TRIANGLES, 0, 6);
#endif
            
            if (shader_list_enabled) {
                for (int i = 0; i < shader_list.size(); ++i) {
                    // not sure how to do this
                }
            }
            
            if (take_snapshot == true) {
                takeSnapshot();
                take_snapshot = false;
            }
            if (writer_set == true)
                writeFrame();
#ifdef SPOUT_SERVER
            if (!bInitialized) {
                // Create the sender
                bInitialized = spoutsender->CreateSender("acidcamGL", window_width, window_height);
            }
            
            if (bInitialized) {
                
                if (window_width > 0 && window_height > 0) { // protect against user minimize
                    // Grab the screen into the local spout texture
                    glBindTexture(GL_TEXTURE_2D, senderTexture);
                    glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, window_width, window_height);
                    glBindTexture(GL_TEXTURE_2D, 0);
                    // Send the texture out for all receivers to use
                    spoutsender->SendTexture(senderTexture, GL_TEXTURE_2D, window_width, window_height);
                }
            }
#endif
            
            if (video_mode && fps != 0) {
                std::chrono::time_point<std::chrono::system_clock> nowx =
                std::chrono::system_clock::now();
                auto m = std::chrono::duration_cast<std::chrono::milliseconds>(nowx - now).count();
                if (fps > 0) {
                    int fps_mil = 1000 / fps;
                    if (m < fps_mil)
                        std::this_thread::sleep_for(std::chrono::milliseconds(fps_mil - m - 1));
                }
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
            if(i != 0) ac_on = true;
            
            if(debug) {
                std::cout << "acidcam: Filter Index: " << index << " - " << ac::solo_filter[index] << "\n";
            }
        }
                
        int setCustomFilter(std::string s) {
            custom_filters["CustomFilterStack"] = std::vector<std::string>();
            std::vector<std::string> &flist = custom_filters["CustomFilterStack"];
            auto pos = s.find(",");
            while(pos != std::string::npos) {
                std::string left, right;
                left = s.substr(0, pos);
                s = s.substr(pos+1, s.length());
                if(find_solo(left) == -1) {
                    std::cout << "acidcam: Error filter: " << left << " not found!\n";
                    updateError();
                }
                flist.push_back(left);
                pos = s.find(",");
                if(pos == std::string::npos && s.length() > 0) {
                    if(find_solo(s) == -1) {
                        std::cout << "acidcam: Error filter: " << s << " not found!\n";
                        updateError();
                    }
                    flist.push_back(s);
                }
            }
            ac::solo_filter.push_back("CustomFilterStack");
            std::cout << "acidcam: Custom Loaded: " <<  ac::solo_filter[ac::solo_filter.size()-1] << "\n";
            return static_cast<int>(ac::solo_filter.size()-1);
            
        }
        
        int setCustomFilterByIndex(std::string s) {
            custom_filters["CustomFilterStack"] = std::vector<std::string>();
            std::vector<std::string> &flist = custom_filters["CustomFilterStack"];
            auto pos = s.find(",");
            while(pos != std::string::npos) {
                std::string left, right;
                left = s.substr(0, pos);
                s = s.substr(pos+1, s.length());
                int num = atoi(left.c_str());
                if(num < 0 || num > ac::solo_filter.size()-1) {
                    std::cout << "acidcam: Error index out of range.\n";
                    updateError();
                    return 0;
                }
                flist.push_back(ac::solo_filter[num]);
                pos = s.find(",");
                if(pos == std::string::npos && s.length() > 0) {
                    int num = atoi(s.c_str());
                    
                    if(num < 0 || num > ac::solo_filter.size()-1) {
                        std::cout << "acidcam: Error filter: " << num << " not found!\n";
                        updateError();
                    }
                    flist.push_back(ac::solo_filter[num]);;
                }
            }
            ac::solo_filter.push_back("CustomFilterStack");
            std::cout << "acidcam: Custom Loaded: " <<  ac::solo_filter[ac::solo_filter.size()-1] << "\n";
            return static_cast<int>(ac::solo_filter.size()-1);
            
        }
        
        std::fstream *stdout_file;
        
        void setFile(std::fstream  *fout) {
            stdout_file = fout;
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
                    case GLFW_KEY_1:
                        if(video_mode == true) {
                            stereo_mode =!stereo_mode;
                            if(stereo_mode)
                                std::cout << "acidcam: Stereo: on\n";
                            else
                                std::cout << "acidcam Stereo: off\n";
                        }
                        break;
                    case GLFW_KEY_SEMICOLON:
                        blur_enabled = !blur_enabled;
                        if(blur_enabled)
                            std::cout << "acidcam: Blur Mode enabled...\n";
                        else
                            std::cout << "acidcam: Blur Mode disabled...\n";
                        break;
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
                        
                        if(mode == GLFW_MOD_SHIFT) {
                            std::cout << "acidcam: Attempting to release objects....\n";
                            ac::release_all_objects();
                            break;
                        }
                        
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
                        if(mode == GLFW_MOD_SHIFT) {
                            if(af.size()>0)
                                af_enabled = !af_enabled;
                            if(af_enabled && af.size()>0)
                                std::cout << "acidcam: Autofilter [On]\n";
                            else if(af_enabled == false && af.size()>0)
                                std::cout << "acidcam: Autofilter [Off]\n";
                        }
                        else {
                            ac_on = !ac_on;
                            if(ac_on)
                                std::cout << "acidcam: filters enabled...\n";
                            else
                                std::cout << "acidcam: filters disabled...\n";
                        }
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
                            
                            if(var_list[var_index] > 0 && var_list[var_index] < ac::solo_filter.size()-1) {
                                index = var_list[var_index];
                                if(debug) {
                                    std::cout << "acidcam: Filter Index: " << index << " - " << ac::solo_filter[index] << "\n";
                                }
                            }
                        }
                        break;
                    case GLFW_KEY_RIGHT:
                        if(list_enabled == false) {
                            if(index < ac::solo_filter.size()-1) {
                                ++index;
                                if(debug) {
                                    std::cout << "acidcam: Filter Index: " << index << " - " << ac::solo_filter[index] << "\n";
                                }
                            }
                        } else {
                            if(var_index < var_list.size()-1)
                                var_index += 1;
                            if(var_list[var_index] > 0 && var_list[var_index] < ac::solo_filter.size()-1) {
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
                        if(af_enabled == false && shader_index > 0) {
                            --shader_index;
                            setShader(shader_index);
                        }
                        break;
                    case GLFW_KEY_DOWN:
                        if(af_enabled == false && shader_index < shaders.size()-1) {
                            shader_index++;
                            setShader(shader_index);
                        }
                        break;
                    case GLFW_KEY_H:
                        if(mode == GLFW_MOD_SHIFT) {
                            if(af.size()>0) {
                                static bool shuffle_on = false;
                                shuffle_on = !shuffle_on;
                                af.shuffle(shuffle_on);
                                af.shuffle();
                            }
                        } else {
                            std::cout << "acidcam: Playlist shuffled...\n";
                            sortPlaylist();
                        }
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
                    case GLFW_KEY_PAGE_UP: {
                        stored_position = index;
                        stored_var_position = var_index;
                        
                        std::string namex;
                        if(stored_position >= 0 && stored_position < ac::solo_filter.size()) {
                            namex = ac::solo_filter[stored_position];
                        } else {
                            namex = "Unknown";
                        }
                        
                        std::cout << "acidcam: Stored position set to: " << namex << " - " << stored_position << " playlist position: " << stored_var_position << "\n";
                    }
                        break;
                    case GLFW_KEY_PAGE_DOWN: {
                        if(list_enabled == false)
                            index = stored_position;
                        else
                            index = var_list[stored_var_position];
                        std::string namex;
                        if(index >= 0 && index < ac::solo_filter.size()) {
                            namex = ac::solo_filter[index];
                        } else {
                            namex = "Unknown";
                        }
                        std::cout << "acidcam: Index set to stored position: " << namex << " - " << index << "\n";
                    }
                        break;
                    case GLFW_KEY_SLASH:
                        rand_shader = !rand_shader;
                        std::cout << "acidcam: Random Shader Toggled...\n";
                        break;
                    case GLFW_KEY_DELETE:
                        shader_list_enabled = !shader_list_enabled;
                        if(shader_list_enabled) {
                            std::cout << "acidcam: shader list enabled: " << shader_list.size() << " shaders.\n";
                        }
                        else
                            std::cout << "acidcam: shader list disabled.\n";
                        
                        break;
                }
            }
#if defined(__APPLE__) || defined(__linux__)
            if(redirect != 0) {
                sendString(redirect->getString());
            }
#endif
        }
        
        void resize(int newWidth, int newHeight) {
            aspect = (float)newWidth / (float)newHeight;
            glViewport(0, 0, newWidth, newHeight);
            p_mat = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f);
            window_width = newWidth;
            window_height = newHeight;
#ifdef SPOUT_SERVER
            INIT_TEXTURE(senderTexture, window_width, window_height);
#endif
#ifdef SYPHON_SERVER
            if(syphon_enabled) {
                syphon_size(width, height);
            }
#endif
        }
        
        //std:::unordered_map<std::string, std::vector<std::string>> custom_filters;
        
        using List = std::vector<std::string>;
        std::unordered_map<std::string, List> custom_filters;
        using Prog = AC_Program *;
        std::unordered_map<std::string, Prog> plugins;

        void CallCustom(std::string index, cv::Mat &frame) {
            std::string val = index;
            
            if(stereo_mode) {
                frame.create(cv::Size(window_width, window_height), CV_8UC3);
                stereo.Render(frame);
            }
            
            if(blur_enabled) {
                ac::MedianBlur(frame);
                ac::MedianBlur(frame);
                ac::MedianBlur(frame);
            }
            
            cv::flip(frame, frame, 0);
            
            
            if(plugins.find(val) != plugins.end()) {
                plugins[val]->exec(frame);
            } else if(custom_filters.find(val) != custom_filters.end()) {
                for(int i = 0; i < custom_filters[val].size(); ++i) {
                    if(plugins.find(custom_filters[val].at(i)) != plugins.end())
                        plugins[custom_filters[val].at(i)]->exec(frame);
                    else
                        ac::CallFilter(custom_filters[val].at(i),frame);
                }
            } else {
                ac::CallFilter(index, frame);
            }
        }
        
        void loadPlugins(std::string c) {
            if(c.length()==0)return;
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
                if(file && n.length() > 0) {
                    AC_Program *program = new AC_Program();
                    if(program->load(c + "/" + n)) {
                        plugins[n] = program;
                        std::cout << "acidcam: Loaded Plugin: " << n << "\n";
                        ac::solo_filter.push_back(n);
                    } else {
                        std::cout << "acidcam: Could not open plugin: " << n << "\n";
                        //acidcam::updateError();
                    }
                }
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
        
        void loadShaderList(std::string lst) {
            std::fstream file;
            file.open(lst, std::ios::in);
            
            if(!file.is_open()) {
                std::cout << "acidcam: Error could not load shader list...\n";
                acidcam::updateError();
            }
            
            while(!file.eof()) {
                std::string s;
                std::getline(file, s);
                if(file)
                    shader_list.push_back(shader_map[s]);
            }
            std::cout << "acidcam: shader list loaded...\n";
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
                    shader_map[s] = index;
                    std::cout << "\n";
                    p.setName(s.substr(0, s.rfind(".")));
                    shaders.push_back(p);
                    ++index;
                }
            }
            file.close();
            std::cout << "acidcam: Loaded " << shaders.size() << " Shaders...\n";
        }
        
        // cube
        
        float cameraX, cameraY, cameraZ;
        float cube_x, cube_y, cube_z;
        glm::mat4 r_mat;
        
        virtual void init_cube() {
            cameraX = 0;
            cameraY = 0;
            cameraZ = 8.0f;
            cube_x = 0.0f;
            cube_y = -2.0f;
            cube_z = 0.0f;
            color_alpha_r = 0.1;
            color_alpha_g = 0.2;
            color_alpha_b = 0.3;
            
            GLfloat vertices[] = { -1.0f, -1.0f, 1.0f, // front face
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                
                -1.0f, -1.0f, -1.0f, // left side
                -1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, -1.0f,
                
                -1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                
                -1.0f, 1.0f, -1.0f, // top
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                
                -1.0f, -1.0f, -1.0f, // bottom
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                
                1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                
                1.0f, -1.0f, -1.0f, // right
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                
                1.0f, 1.0f, -1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                
                -1.0f, -1.0f, -1.0f, // back face
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
            };
            
            GLfloat texCoords[] = {
                0, 0, // front
                1, 1,
                0, 1,
                
                0, 0,
                1, 0,
                1, 1,
                
                0, 0, // left
                1, 0,
                0, 1,
                
                0, 1,
                1, 0,
                1, 1,
                
                0,0, // top
                0,1,
                1,1,
                
                1, 1,
                1, 0,
                0, 0,
                
                0, 0,// bottom
                0, 1,
                1, 1,
                
                1,1,
                1,0,
                0,0,
                
                0,0,// right
                1,0,
                0,1,
                
                0,1,
                1,0,
                1,1,
                
                0,0, // back
                1,1,
                0,1,
                
                0,0,
                1,0,
                1,1
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
            
            p_mat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
            
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            cv::Mat frame;
            cap.read(frame);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.ptr());
        }
        
        virtual void update_cube(double timeval) {
            glClearColor(0.0, 0.0, 0.0, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            program.useProgram();
            
            mv_loc = glGetUniformLocation(program.id(), "mv_matrix");
            proj_loc = glGetUniformLocation(program.id(),"proj_matrix");
           
            GLuint samp = glGetUniformLocation(program.id(), "samp");
            GLuint mat_samp = glGetUniformLocation(program.id(), "mat_samp");
            GLuint calpha_r = glGetUniformLocation(program.id(), "value_alpha_r");
            GLuint calpha_g = glGetUniformLocation(program.id(), "value_alpha_g");
            GLuint calpha_b = glGetUniformLocation(program.id(), "value_alpha_b");
            GLuint c_index = glGetUniformLocation(program.id(), "index_value");
            GLuint c_tf = glGetUniformLocation(program.id(), "time_f");
            GLuint alpha_pos = glGetUniformLocation(program.id(), "alpha_value");
            GLuint optx_pos = glGetUniformLocation(program.id(), "optx");
            GLuint rand_pos = glGetUniformLocation(program.id(), "random_var");
            GLuint restore_blackx = glGetUniformLocation(program.id(), "restore_black");
            GLuint inc_value_pos = glGetUniformLocation(program.id(), "inc_value");
            GLuint inc_value_posx = glGetUniformLocation(program.id(), "inc_valuex");
            
            
            cameraZ = 8.0f;
            
            v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, cameraY, -cameraZ));
            cv::Mat frame;
            if (!cap.read(frame)) {
                if (repeat == true && repeat_filename.length() > 0) {
                    cap.open(repeat_filename);
                    if (cap.isOpened()) {
                        cap.read(frame);
                        std::cout << "acidcam: video loop...\n";
                    }
                    else {
                        std::cout << "acidcam: Capture device closed exiting...\n";
                        quit();
                        return;
                    }
                }
                else {
                    std::cout << "acidcam: Capture device closed exiting...\n";
                    quit();
                    return;
                }
            }
            if (shader_index == 0 || ac_on == true) {
                if (index >= 0 && index < ac::solo_filter.size()) {
                    cv::Mat orig;
                    
                    if (blend_index > 0) {
                        orig = frame.clone();
                    }
                    
                    CallCustom(ac::solo_filter[index], frame);
                    
                    if (blend_index > 0 && blend_index / 10 > 0) {
                        cv::Mat copyf;
                        double per = 1.0 / (blend_index / 10);
                        ac::AlphaBlendDouble(frame, orig, copyf, per, 1.0 - per);
                        frame = copyf.clone();
                    }
                    
                    if (color_map != -1) {
                        cv::Mat output_f1 = frame.clone();
                        cv::applyColorMap(output_f1, frame, color_map);
                    }
                    
                    if (restore_black == true) {
                        ac::CallFilter("RestoreBlack", frame);
                        if (color_map != -1) {
                            cv::Mat output_f1 = frame.clone();
                            cv::applyColorMap(output_f1, frame, color_map);
                        }
                    }
                    if (print_text == true) {
                        std::ostringstream stream;
                        stream << ac::solo_filter[index];
                        cv::putText(frame, stream.str(), cv::Point(40, 40), cv::FONT_HERSHEY_DUPLEX, 1.0, CV_RGB(255, 255, 255), 2);
                    }
                }
            }
            cv::flip(frame, frame, 0);
            cameraZ += -5.0f;
            v_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, cameraY, -cameraZ));
            m_mat = glm::translate(glm::mat4(1.0f),glm::vec3(0.0f,0.0f,0.0f));
            r_mat = glm::rotate(glm::mat4(1.0f), 1.75f*(float)timeval,glm::vec3(0.0f, 1.0f, 0.0f));
            r_mat = glm::rotate(r_mat, 1.75f*(float)timeval,glm::vec3(1.0f, 0.0f, 0.0f));
            r_mat = glm::rotate(r_mat, 1.75f*(float)timeval, glm::vec3(0.0f, 0.0f, 1.0f));
            
            m_mat = m_mat * r_mat;
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
            
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
            
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            color_alpha_r += rand() % 100 * 0.01f;
            color_alpha_g += rand() % 100 * 0.01f;
            color_alpha_b += rand() % 100 * 0.01f;
            
            if (color_alpha_r > 1.5f)
                color_alpha_r = 0.1f;
            if (color_alpha_g > 1.5f)
                color_alpha_g = 0.1f;
            if (color_alpha_b > 1.5f)
                color_alpha_b = 0.1f;
            static bool idir = true;
            if (idir == true) {
                alpha += 0.1f;
                if (alpha >= 6.0)
                    idir = false;
            }
            else {
                alpha -= 0.1f;
                if (alpha <= 1.0f) {
                    idir = true;
                }
                
            }
            random_var = glm::vec4(rand() % 255, rand() % 255, rand() % 255, rand() % 255);

            static int inc_dir = 1;
            if (inc_dir == 1) {
                for (int i = 0; i < 3; ++i) {
                    ++inc_value[i];
                    if (inc_value[i] >= 255)
                        inc_dir = 0;
                }
            }
            else {
                for (int i = 0; i < 3; ++i) {
                    --inc_value[i];
                    if (inc_value[i] <= 1) {
                        inc_dir = 1;
                        inc_value = glm::vec4(rand() % 255, rand() % 255, rand() % 255, 1);
                    }
                }
            }
            static int inc_dirx = 1;
            if (inc_dirx == 1) {
                for (int i = 0; i < 3; ++i) {
                    ++inc_valuex[i];
                    if (inc_valuex[i] >= 255)
                        inc_dirx = 0;
                }
            }
            else {
                for (int i = 0; i < 3; ++i) {
                    --inc_valuex[i];
                    if (inc_valuex[i] <= 1) {
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
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_LEQUAL);
            
            glDrawArrays(GL_TRIANGLES,0,36);
            
            if (take_snapshot == true) {
                takeSnapshot();
                take_snapshot = false;
            }
            if (writer_set == true)
                writeFrame();
        }
    
        GLuint createShaderProgram() {
            GLuint vfProgram = program.createProgramFromFile("vertex.glsl", "frag.glsl");
            return vfProgram;
        }
        
    };
}
