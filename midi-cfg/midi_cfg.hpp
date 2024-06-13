#ifndef _MIDI_KEY_H_
#define _MIDI_KEY_H_

#include<iostream>
#include<vector>
#include<string>
#include<unordered_map>

namespace midi {

    struct Key {
        std::vector<unsigned char> bytes;
        Key() = default;
        Key(unsigned char byte1, unsigned char byte2, unsigned char byte3);
    };

    class MIDI_Config {
    public:
        void addCode(int key, const Key &k);
        void write(const std::string &filename);
        void read(const std::string &filename);
        int lookup(const Key &k);
        void print(std::ostream &out);
    protected:
        std::unordered_map<int, Key> codes;
    };
}


#endif