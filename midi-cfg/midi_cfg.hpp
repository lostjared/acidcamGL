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
        Key(unsigned char status, unsigned char key, unsigned char velocity);
    };

    class MIDI_Config {
    public:
        MIDI_Config() = default;
        void addCode(int key, const Key &k);
        void write(const std::string &filename);
        void read(const std::string &filename);
        int lookup(const Key &k);
        void print(std::ostream &out);
    protected:
        std::unordered_multimap<int, Key> codes;
    };
}


#endif
