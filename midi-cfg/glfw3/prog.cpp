#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <array>

int main() {
    std::vector<std::array<std::string, 2>> keyMappings = {
        {"GLFW_KEY_ESCAPE", std::to_string(GLFW_KEY_ESCAPE)},
        {"GLFW_KEY_1", std::to_string(GLFW_KEY_1)},
        {"GLFW_KEY_SEMICOLON", std::to_string(GLFW_KEY_SEMICOLON)},
        {"GLFW_KEY_COMMA", std::to_string(GLFW_KEY_COMMA)},
        {"GLFW_KEY_PERIOD", std::to_string(GLFW_KEY_PERIOD)},
        {"GLFW_KEY_L", std::to_string(GLFW_KEY_L)},
        {"GLFW_KEY_N", std::to_string(GLFW_KEY_N)},
        {"GLFW_KEY_P", std::to_string(GLFW_KEY_P)},
        {"GLFW_KEY_K", std::to_string(GLFW_KEY_K)},
        {"GLFW_KEY_J", std::to_string(GLFW_KEY_J)},
        {"GLFW_KEY_Z", std::to_string(GLFW_KEY_Z)},
        {"GLFW_KEY_ENTER", std::to_string(GLFW_KEY_ENTER)},
        {"GLFW_KEY_F", std::to_string(GLFW_KEY_F)},
        {"GLFW_KEY_S", std::to_string(GLFW_KEY_S)},
        {"GLFW_KEY_C", std::to_string(GLFW_KEY_C)},
        {"GLFW_KEY_SPACE", std::to_string(GLFW_KEY_SPACE)},
        {"GLFW_KEY_LEFT", std::to_string(GLFW_KEY_LEFT)},
        {"GLFW_KEY_RIGHT", std::to_string(GLFW_KEY_RIGHT)},
        {"GLFW_KEY_M", std::to_string(GLFW_KEY_M)},
        {"GLFW_KEY_MINUS", std::to_string(GLFW_KEY_MINUS)},
        {"GLFW_KEY_EQUAL", std::to_string(GLFW_KEY_EQUAL)},
        {"GLFW_KEY_UP", std::to_string(GLFW_KEY_UP)},
        {"GLFW_KEY_DOWN", std::to_string(GLFW_KEY_DOWN)},
        {"GLFW_KEY_H", std::to_string(GLFW_KEY_H)},
        {"GLFW_KEY_G", std::to_string(GLFW_KEY_G)},
        {"GLFW_KEY_Q", std::to_string(GLFW_KEY_Q)},
        {"GLFW_KEY_W", std::to_string(GLFW_KEY_W)},
        {"GLFW_KEY_E", std::to_string(GLFW_KEY_E)},
        {"GLFW_KEY_R", std::to_string(GLFW_KEY_R)},
        {"GLFW_KEY_Y", std::to_string(GLFW_KEY_Y)},
        {"GLFW_KEY_U", std::to_string(GLFW_KEY_U)},
        {"GLFW_KEY_I", std::to_string(GLFW_KEY_I)},
        {"GLFW_KEY_O", std::to_string(GLFW_KEY_O)},
        {"GLFW_KEY_T", std::to_string(GLFW_KEY_T)},
        {"GLFW_KEY_PAGE_UP", std::to_string(GLFW_KEY_PAGE_UP)},
        {"GLFW_KEY_PAGE_DOWN", std::to_string(GLFW_KEY_PAGE_DOWN)},
        {"GLFW_KEY_SLASH", std::to_string(GLFW_KEY_SLASH)},
        {"GLFW_KEY_DELETE", std::to_string(GLFW_KEY_DELETE)},
    };

    for (const auto& keyMapping : keyMappings) {
        std::cout << keyMapping[0] << ": " << keyMapping[1] << std::endl;
    }

    return 0;
}