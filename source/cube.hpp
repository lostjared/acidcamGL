
static constexpr int numVAOs = 1;
static constexpr int numVBOs = 2;
std::string filename;

class MainWindow : public glWindow {
    GLuint renderingProgram;
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
    
    cv::VideoCapture cap;
    float color_alpha_r, color_alpha_g, color_alpha_b;
public:
    

