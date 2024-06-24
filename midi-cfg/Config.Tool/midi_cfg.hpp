#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace std {
    template <>
    struct hash<std::pair<int, int>> {
        size_t operator()(const std::pair<int, int>& p) const {
            return hash<int>()(p.first) ^ hash<int>()(p.second);
        }
    };
}

namespace midi {


    struct Key {
        std::vector<unsigned char> bytes;

        Key() = default;
        Key(unsigned char byte1, unsigned char byte2, unsigned char byte3) {
            bytes.push_back(byte1);
            bytes.push_back(byte2);
            bytes.push_back(byte3);
        }
    };

    class MIDI_Config {
    public:
        void addCode(std::pair<int, int> keys, const Key& k) {
            codes.insert({ keys, k });
        }

        void write(const std::string& filename) {
            std::fstream file;
            file.open(filename, std::ios::out);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open file");
            }
            for (auto it = codes.begin(); it != codes.end(); ++it) {
                file << it->first.first << ":" << it->first.second << " {";
                for (int j = 0; j < it->second.bytes.size() - 1; ++j) {
                    file << static_cast<int>(it->second.bytes[j]) << " ";
                }
                file << static_cast<int>(it->second.bytes[it->second.bytes.size() - 1]) << "}\n";
            }
            file.close();
        }

        void read(const std::string& filename) {
            std::fstream file;
            file.open(filename, std::ios::in);
            if (!file.is_open()) {
                throw std::runtime_error("Could not open file for reading");
            }

            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string keyPair;
                if (!(iss >> keyPair)) {
                    continue;
                }

                size_t colonPos = keyPair.find(':');
                if (colonPos == std::string::npos) {
                    continue;
                }

                int key1 = std::stoi(keyPair.substr(0, colonPos));
                int key2 = std::stoi(keyPair.substr(colonPos + 1));

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
                    bytes.pop_back(); 
                }
                addCode({ key1, key2 }, Key(bytes[0], bytes[1], bytes[2]));
            }
            file.close();
            print(std::cout);
        }

        std::pair<int, int> lookup(const Key& k) {
            for (auto it = codes.begin(); it != codes.end(); ++it) {
                if (it->second.bytes.size() >= k.bytes.size()) {
                    if (k.bytes.at(0) == it->second.bytes.at(0) && k.bytes.at(1) == it->second.bytes.at(1) && k.bytes.at(2) == it->second.bytes.at(2)) {
                        return it->first;
                    }
                }
            }
            return { -1, -1 };
        }

        std::pair<int, int> lookup_nov(const Key& k) {
            for (auto it = codes.begin(); it != codes.end(); ++it) {
                if (it->second.bytes.size() >= k.bytes.size()) {
                    if (k.bytes.at(0) == it->second.bytes.at(0) && k.bytes.at(1) == it->second.bytes.at(1)) {
                        return it->first;
                    }
                }
            }
            return { -1, -1 };
        }

        void print(std::ostream& out) {
            for (auto it = codes.begin(); it != codes.end(); ++it) {
                out << "acidcam: Mapped Function Keys: " << it->first.first << ":" << it->first.second << " Bytes: ";
                for (int j = 0; j < it->second.bytes.size(); ++j) {
                    out << static_cast<int>(it->second.bytes[j]) << " ";
                }
                out << "\n";
            }
        }

    protected:
        std::unordered_multimap<std::pair<int, int>, Key, std::hash<std::pair<int, int>>> codes;
    };
}
