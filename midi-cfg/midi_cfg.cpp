#include"midi_cfg.hpp"
#include<fstream>
#include<sstream>

namespace midi {


Key::Key(unsigned char byte1, unsigned char byte2, unsigned char byte3) {
    bytes.push_back(byte1);
    bytes.push_back(byte2);
    bytes.push_back(byte3);
}

void MIDI_Config::addCode(int key, const Key &k) {
    codes[key] = k;
}

void MIDI_Config::write(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::out);
    if(!file.is_open()) {
        throw std::runtime_error("Could not open file");
    }
    for(auto it = codes.begin(); it != codes.end(); ++it) {
        file << it->first << " " << "{";
        for(int j = 0; j < it->second.bytes.size()-1; ++j) {
            file << static_cast<int>(it->second.bytes[j]) << " ";
        }
        file << static_cast<int>(it->second.bytes[it->second.bytes.size()-1]) << "}\n";
    }
    file.close();
}

void MIDI_Config::read(const std::string &filename) {
    std::fstream file;
    file.open(filename, std::ios::in);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for reading");
    }
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key;
        if (!(iss >> key)) {
            continue; 
        }   
        char brace;
        if (!(iss >> brace) || brace != '{') {
            continue; 
        }
        
        std::vector<unsigned char> bytes;
        int byteValue;
        while (iss >> byteValue) {
            bytes.push_back(static_cast<unsigned char>(byteValue));
        }
        
        if (!bytes.empty() && bytes.back() == '}') {
            bytes.pop_back(); // Remove the closing brace '}' from the bytes vector
        }
        addCode(atoi(key.c_str()), Key(bytes[0], bytes[1], bytes[2]));
    }
    file.close();
}

int MIDI_Config::lookup(const Key &k) {
    for(auto it = codes.begin(); it != codes.end(); ++it) {
        if(it->second.bytes.size() >= k.bytes.size()) {
            bool t = true;
            for(int i = 0; i < k.bytes.size(); ++i) {
                if(k.bytes.at(i) != it->second.bytes.at(i)) {
                    t = false;
                }
            }
            if(t == true) {
                return it->first;
            }
        }
    }
    return -1;
}

void MIDI_Config::print(std::ostream &out) {
    for(auto it = codes.begin(); it != codes.end(); ++it) {
        out << "Key: " << it->first << " Bytes: ";
        for(int j = 0; j < it->second.bytes.size(); ++j) {
            out << static_cast<int>(it->second.bytes[j]) <<  " ";
        }
        out << "\n";
    }
}


}

