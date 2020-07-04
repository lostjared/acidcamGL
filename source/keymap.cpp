#include"keymap.hpp"
#include<sstream>
#include<cstdlib>
#include<cctype>

namespace acidcam {
    
    void KeyMap::load(const std::string &name) {
        std::fstream file;
        file.open(name, std::ios::in);
        if(!file.is_open()) {
            std::cerr << "Error opening file: " << name << "\n";
            exit(EXIT_FAILURE);
        }
        
        while(!file.eof()) {
            std::string s;
            std::getline(file, s);
            if(file) {
                std::istringstream stream(s);
                char key = 0;
                int filt = 0, shade = 0;
                stream >> key >> filt >> shade;
                if(key > 0) {
                    key = toupper(key);
                    setKey(key, filt, shade);
                    std::cout << "Shortcut Key Set: " << key << "\n";
                }
            }
        }
    }
    
    void KeyMap::setKey(int k, int s, int i) {
        keys[toupper(k)] = std::pair<int,int>(s, i);
    }
    
    bool KeyMap::checkKey(int key, int &s, int &i) {
        auto pos = keys.find(toupper(key));
        if(pos == keys.end())
            return false;
        s = pos->second.first;
        i = pos->second.second;
        return true;
    }

}
