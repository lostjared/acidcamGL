
#include"acidcam_window.hpp"

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

constexpr unsigned long outstr_size = 49;
std::string outstr_arr[] = {
    "Written by Jared Bruni",
    "    http://lostsidedead.com",
    "    Arguments:",
    "    -h use H.264 output w/o uses MPEG-4",
    "    -o output mp4 filename",
    "    -S filter start index",
    "    -H shader start index",
    "    -C set color map",
    "    -T set material texture filename",
    "    -k shortcut-key file",
    "    -L playlist of filters",
    "    -b restore black",
    "    -g output debug strings",
    "    -u fps",
    "    -n print filter name",
    "    -e snapshot prefix",
    "    -p shader path",
    "    -M monitor index",
    "    -f fullscreen (resize)",
    "    -F fullscreen (windowed mode)",
    "    -d capture device",
    "    -i input_video.mp4",
    "    -R loop input video",
    "    -r resolution 1920x1080",
    "    -c Camera resolution 1280x720",
    "    -l list filters",
    "    -t list filters no info",
    "    -l list search",
    "    -v version",
    "    ",
    "Controls:",
    "    ",
    "    L - enable disable playlist",
    "    N - set index to the end",
    "    P - index reset to zero",
    "    K - jump forward index by 25",
    "    J - jump backwar index by 25",
    "    Z - take screenshot",
    "    F - process keyboard input for index",
    "    S - process keyboard input for shader",
    "    C - clear keyboard input",
    "    [SPACE] - Acid Cam filters enabled/disabled",
    "    [LEFT] - Filter index move left",
    "    [RIGHT] - filter index move right",
    "    [UP] - Shader Index move up",
    "    [DOWN] - Shader Index move down",
    "    [ENTER] - Jump to currently typed index (type index with number keys at anytime)",
    "     Q,Q,E,R,Y,U,I,O - move movement rate",
    "     T - reset color offset"
};

void print_help_message() {
    for(int i = 0; i < outstr_size; ++i)
        std::cout << outstr_arr[i] << "\n";
}

int findFilter(std::string f) {
    for(int i = 0; i < ac::solo_filter.size(); ++i) {
         if(f == ac::solo_filter[i])
            return i;
    }
    return -1;
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
    int start_shader = 0;
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
    std::string filter_string;
    bool h264 = false;
    bool force_full = false;
    int monitor = 0;
    int set_index = 0;
    bool repeat = false;
    int color_map = -1;
    std::string material;
    while((opt = getopt(argc, argv, "T:C:Z:H:S:M:Fhbgu:p:i:c:r:Rd:fhvj:snlk:e:L:o:tQ:")) != -1) {
        switch(opt) {
            case 'T':
                material  = optarg;
                break;
            case 'C':
                color_map = atoi(optarg);
                break;
            case 'Z':
                filter_string = optarg;
                break;
            case 'H':
                start_shader = atoi(optarg);
                break;
            case 'R':
                repeat = true;
                break;
            case 'S':
                set_index = atoi(optarg);
                if(set_index < 0 || set_index > ac::solo_filter.size()-1) {
                    std::cout << "acidcam: Error invalid starting index...\n";
                    exit(EXIT_FAILURE);
                }
                break;
            case 'M':
                monitor = atoi(optarg);
                break;
            case 'F':
                force_full = true;
                break;
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
            case 'Q': {
                for(int i = 0; i < ac::solo_filter.size(); ++i) {
                    if(ac::solo_filter[i].find(optarg) != std::string::npos)
                        std::cout << ac::solo_filter[i] << "\n";
                }
                exit(EXIT_SUCCESS);
            }
                break;
            case 't':
                for(int i = 0; i < ac::solo_filter.size(); ++i) {
                    std::cout << ac::solo_filter[i] << "\n";
                }
                exit(EXIT_SUCCESS);
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
        if(output_file.length()>0) {
            std::cerr << "acidcam: Error outuput file not supported for camera mode use OBS.\n";
            output_file = "";
            exit(EXIT_SUCCESS);
        }
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
    if(force_full == true) {
        if(output_file.length()>0) {
            std::cerr << "fullscreen mode not suppported in video record mode, use OBS to record..\n";
            exit(EXIT_FAILURE);
        }
        main_window.create(false, true, false,"acidcamGL", w, h, monitor);
    }
    else {
        main_window.create(((output_file.length()>0)?true:false), full,true, "acidcamGL", w, h, monitor);
    }
    std::cout << "acidcam: GL Version: " << glGetString(GL_VERSION) << "\n";
    std::cout << "acidcam: Actual " << ((filename.length()==0) ? "Camera" : "File") << " Resolution: " << cw << "x" << ch << "p" << fps << " \n";
    glfwSetKeyCallback(main_window.win(), key_callback);
    glfwSetWindowSizeCallback(main_window.win(), window_size_callback);
    glfwSetCharCallback(main_window.win(), character_callback);
    if(list_var.length()>0)
        main_window.loadList(list_var);
    main_window.setDebug(debug_val);
    main_window.setRepeat(filename, repeat);
    
     if(material.length()>0)
           main_window.genMaterial(material);
        
    main_window.loadShaders(shader_path);
    main_window.setShader(start_shader);
    if(filter_string.length()>0) {
        set_index = findFilter(filter_string);
        if(set_index == -1) {
            std::cerr << "acidcam: Error could not find filter: " << filter_string << "\n";
            exit(EXIT_FAILURE);
        }
    }
    main_window.setFilterIndex(set_index);
    main_window.setPrintText(print_text);
    main_window.setPrefix(snapshot_prefix);
    main_window.setRestoreBlack(restore_black);
    main_window.setColorMap(color_map);
     
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
