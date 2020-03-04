#include "SDL.h"
#include "GL/glew.h"
#include"acidcam/ac.h"
#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<unistd.h>


static const std::string version_info="v1.0";


GLfloat frontFace[] = {
    -1.0f, -1.0f, 1.0f, // front face
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, 1.0f, 1.0f
};

GLfloat frontTexture[] = {
    0, 0, // front
    1, 1,
    0, 1,
    
    0, 0,
    1, 0,
    1, 1,
};

cv::VideoCapture cap;
GLuint background_texture;
void genTextureFromMat(cv::Mat &frame, GLuint &tex);

void new_gluPerspective( GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar )
{
    const GLdouble pi = 3.1415926535897932384626433832795;
    GLdouble fW, fH;
    fH = tan( fovY / 360 * pi ) * zNear;
    fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );
}

void new_gluOrtho2D(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top)
{
    glOrtho(left, right, bottom, top, -1, 1);
}

void new_gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,GLfloat centerx, GLfloat centery, GLfloat centerz,GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {                   /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    glMultMatrixf(m);
    glTranslatef(-eyex, -eyey, -eyez);
}

SDL_Window *window;
SDL_GLContext context;
int width, height;
SDL_Joystick *stick = 0;

void resize(int w, int h) {
    if(w <= 0 || h <= 0) w = 1, h = 1;
    width = w, height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    new_gluPerspective(45.0f, float(w / h), 0.1f, 300.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void genTextureFromMat(cv::Mat &frame, GLuint &tex) {
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, frame.cols, frame.rows, 0, GL_RGBA, GL_UNSIGNED_BYTE, frame.ptr());
}

void render();
int current_filter = 0;
std::string filename;

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
                std::cout << "acidcamGL " << version_info << " arguments:\n-f fullscreen\n-d capture device\n-r resolution 1920x1080\n-c Camera resolution 1280x720\n-v version\n-h help message\n\n";
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
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cerr << "Error initilizing SDL: " << SDL_GetError() << "\n";
        exit(-1);
    }
    
    if(joy_index != -1) {
        stick = SDL_JoystickOpen(joy_index);
        if(!stick) {
            std::cerr << "acidcamGL: error could not open joystick: " << joy_index << "\n";
        }
        SDL_JoystickEventState(SDL_ENABLE);
    }
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    int full_val = 0;
    if(full == true)
        full_val = SDL_WINDOW_FULLSCREEN_DESKTOP;
    window = SDL_CreateWindow("acidcamGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, full_val | SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);
    glewExperimental = GL_TRUE;
    glewInit();
    std::cout << "acidcamGL " << version_info << ": loaded\n";
    std::cout << "GL Version: " << glGetString(GL_VERSION) << "\n";
    SDL_GL_MakeCurrent(window, context);
    SDL_GetWindowSize(window, &width, &height);
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
    std::cout << "Final Capture Resolution: " << cw << "x" << ch << "\n";
    cv::Mat frame;
    cap.read(frame);
    genTextureFromMat(frame, background_texture);
    glClearColor(0, 0, 0, 1);
    glClearDepth(1.0f);
    resize(width, height);
    SDL_Event e;
    int max_filter = static_cast<int>(ac::solo_filter.size());
    bool active = true;
    std::string num;
    while(active) {
        render();
        while(SDL_PollEvent(&e)) {
            switch(e.type) {
                case SDL_QUIT:
                    active = false;
                    break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_ESCAPE)
                        active = false;
                    
                    switch(e.key.keysym.sym) {
                        case SDLK_EQUALS:
                            if(current_filter + 10 < static_cast<int>(ac::solo_filter.size())) {
                                current_filter += 10;
                                std::cout << "Filter Changed to: [" << current_filter << "/" << max_filter << "] " << ac::solo_filter[current_filter] << "\n";

                            }
                            
                            break;
                        case SDLK_MINUS:
                            if(current_filter - 10 > 0) {
                                current_filter -= 10;
                                std::cout << "Filter Changed to: [" << current_filter << "/" << max_filter << "] " << ac::solo_filter[current_filter] << "\n";
                            }
                            break;
                        case SDLK_LEFT: {
                            if(ac::alpha_increase > 0)
                                ac::alpha_increase -= 0.05;
                            std::cout << "Alpha decreased: " << ac::alpha_increase << "\n";
                        }
                            break;
                        case SDLK_RIGHT: {
                            if(ac::alpha_increase < 4.0)
                                ac::alpha_increase += 0.05;
                            std::cout << "Alpha increased: " << ac::alpha_increase << "\n";
                                                    
                        }
                            break;
                        case SDLK_UP:
                            if(current_filter > 0) {
                                --current_filter;
                                std::cout << "Filter Changed to: [" << current_filter << "/" << max_filter << "] " << ac::solo_filter[current_filter] << "\n";
                                                        
                            }
                            break;
                        case SDLK_DOWN:
                            if(current_filter < static_cast<int>(ac::solo_filter.size()-1)) {
                                current_filter++;
                                std::cout << "Filter Changed to: [" << current_filter << "/" << max_filter << "] " << ac::solo_filter[current_filter] << "\n";

                            }
                            break;
                    }
                    
                case SDL_JOYBUTTONDOWN:
                    switch(e.jbutton.button) {
                        case 0:
                            if(stick != 0 && current_filter > 0) {
                                --current_filter;
                            }
                            break;
                        case 1:
                            if(stick != 0 && current_filter < static_cast<int>(ac::solo_filter.size()-1)) {
                                current_filter++;
                            }
                            break;
                        case 2:
                            if(stick != 0 && ac::alpha_increase > 0) {
                                ac::alpha_increase -= 0.05;
                            }
                            break;
                        case 3:
                            if(stick != 0 && ac::alpha_increase < 4.0) {
                                ac::alpha_increase += 0.05;
                            }
                            break;
                        default:
                            break;
                    }
                    
                case SDL_JOYBUTTONUP:
                    break;
            }
        }
        SDL_GL_SwapWindow(window);
    }
    glDeleteTextures(1, &background_texture);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    if(stick != 0) SDL_JoystickClose(stick);
    SDL_Quit();
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    new_gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // draw in 2D
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, background_texture);
    cv::Mat frame;
    if(!cap.read(frame)) {
        if(filename.length()==0) {
            std::cout << "acidcamGL: capture device closed...\n";
            exit(EXIT_SUCCESS);
        }
        else {
            cap.open(filename);
            if(!cap.isOpened()) {
                std::cerr << "Error loading file...\n";
                exit(EXIT_FAILURE);
            }
            cap.read(frame);
        }
    }
    cv::Mat out = frame.clone();
    cv::flip(out, frame, 0);
    ac::CallFilter(ac::solo_filter[current_filter], frame);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
    glDisable(GL_DEPTH_TEST);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, frontFace);
    glTexCoordPointer(2, GL_FLOAT, 0, frontTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

