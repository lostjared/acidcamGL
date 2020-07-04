#ifndef __KEYMAP__H__
#define __KEYMAP__H__

#include<iostream>
#include<fstream>
#include<unordered_map>
#include<string>

namespace acidcam {

    class KeyMap {
    public:
        void load(const std::string &name);
        void setKey(int k, int s, int i);
        bool checkKey(int key, int &s, int &i);
        
    private:
        std::unordered_map<int,std::pair<int, int>> keys;
    };
    
}

#endif

