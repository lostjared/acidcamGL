#include"gl_shader.hpp"
#include<sstream>
#include<fstream>

int ShaderProgram::printShaderLog(GLuint shader) {
    int len = 0;
    int ch = 0;
    char *log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = new char [len+1];
        glGetShaderInfoLog(shader, len, &ch, log);
        std::cout << "Shader: " << log << "\n";
        delete [] log;
    }
    return 0;
}
void ShaderProgram::printProgramLog(int p) {
    int len = 0;
    int ch = 0;
    char *log;
    glGetShaderiv(p, GL_INFO_LOG_LENGTH, &len);
    if(len > 0) {
        log = new char [len+1];
        glGetProgramInfoLog(p, len, &ch, log);
        std::cout << "Program: " << log << "\n";
        delete [] log;
    }
}
bool ShaderProgram::checkError() {
    bool e = false;
    int glErr = glGetError();
    while(glErr != GL_NO_ERROR) {
        std::cout << "GL Error: " << glErr << "\n";
        e = true;
        glErr = glGetError();
    }
    return e;
}

GLuint ShaderProgram::createProgram(const char *vshaderSource, const char *fshaderSource) {
    GLint vertCompiled;
    GLint fragCompiled;
    GLint linked;
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(vShader, 1, &vshaderSource, NULL);
    glShaderSource(fShader, 1, &fshaderSource, NULL);
    
    glCompileShader(vShader);
    checkError();
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
    
    if(vertCompiled != 1) {
        std::cout << "Error on Vertex compile\n";
        printShaderLog(vShader);
    }
    
    glCompileShader(fShader);
    checkError();
    
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
    
    if(fragCompiled != 1) {
        std::cout << "Error on Fragment compile\n";
        printShaderLog(vShader);
    }
    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);
    checkError();
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if(linked != 1) {
        std::cout << "Linking failed...\n";
        printProgramLog(vfProgram);
    }
    return vfProgram;
}

GLuint ShaderProgram::createProgramFromFile(const std::string &vert, const std::string &frag) {
    std::fstream v,f;
    v.open(vert, std::ios::in);
    if(!v.is_open()) {
        std::cerr << "Error could not open file: " << vert << "\n";
        exit(EXIT_FAILURE);
    }
    f.open(frag, std::ios::in);
    if(!f.is_open()) {
        std::cerr << "Eror could not open file: " << frag << "\n";
        exit(EXIT_FAILURE);
    }
    std::ostringstream stream1, stream2;
    stream1 << v.rdbuf();
    stream2 << f.rdbuf();
    return createProgram(stream1.str().c_str(), stream2.str().c_str());
}

bool ShaderProgram::loadProgram(const std::string &v, const std::string &f) {
    shader_id = createProgramFromFile(v,f);
    if(shader_id)
        return true;
    return false;
}

void ShaderProgram::useProgram() {
    glUseProgram(shader_id);
}
