#ifndef __AUTOFILTER_H__
#define __AUTOFILTER_H__

#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include<chrono>
#include<algorithm>
#include<random>

namespace acidcam {

    class AutoFilterIndex {
        public:
        std::string shader, filter;
        int frame_index, frame_count;

        AutoFilterIndex(std::string s, std::string f, int fc) : shader{s}, filter{f}, frame_count{fc}, frame_index{0} {}
        AutoFilterIndex() : frame_count{0}, frame_index{0} {}
        AutoFilterIndex(const AutoFilterIndex &i) : shader{i.shader}, filter{i.filter}, frame_count{i.frame_count}, frame_index{i.frame_index} {}
        AutoFilterIndex(AutoFilterIndex &&i) : shader{std::move(i.shader)}, filter{std::move(i.filter)}, frame_count{i.frame_count}, frame_index{i.frame_index} {}

        AutoFilterIndex &operator=(const AutoFilterIndex &i) {
            shader = i.shader;
            filter = i.filter;
            frame_count = i.frame_count;
            frame_index = i.frame_index;
            return *this;
        }

        AutoFilterIndex &operator=(AutoFilterIndex &&i) {
            shader = std::move(i.shader);
            filter = std::move(i.filter);
            frame_count = i.frame_count;
            frame_index = i.frame_index;
            return *this;
        }
        
        friend std::ostream &operator<<(std::ostream &out, const AutoFilterIndex &i);
    };

    inline std::ostream &operator<<(std::ostream &out, const AutoFilterIndex &i) {
        out  << "Shader: " << i.shader << " Filter: " << i.filter << " Count: " << i.frame_count;
        return out;
    }

    class AutoFilter {
    public:
        AutoFilter() : current_index{0} {}

        void clear() {
            if(!playback.empty()) {
                playback.erase(playback.begin(), playback.end());
                current_index = 0;
            }
        }


        void printFilters() {
            int index = 0;
            for(auto &i : playback) {
                std::cout <<"acidcam: [AutoFilter] - index: " << index << " " << i << "\n";
                ++index;
            }
        }

        unsigned int size() const {
            return playback.size();
        }

        bool increment() {
            if(playback[current_index].frame_index < playback[current_index].frame_count-1) {
                playback[current_index].frame_index++;
                return true;
            }
            playback[current_index].frame_index = 0;
            return false;
        }

        AutoFilterIndex current() const {
            return playback[current_index];
        }

        bool next(AutoFilterIndex &index) {
            if(current_index < size()) {
                index = playback[current_index];
                current_index++;
                return true;
            }
            current_index = 0;
            index = playback[current_index];
            return false;
        }

        bool next() {
            if(current_index < size()-1) {
                current_index++;
                return true;
            }
            current_index = 0;
            return false;
        }

        void shuffle() {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            std::shuffle(playback.begin(), playback.end(), std::default_random_engine(seed));
        }
        

        bool loadFile(const std::string &filename) {
            std::fstream file;
            file.open(filename, std::ios::in);
            if(!file.is_open()) return false;
            int line_index = 0;
            while(!file.eof()) {
                std::string line;
                std::getline(file, line);
                if(file && line.length() > 0)
                    parseLine(++line_index, line);
            }
            file.close();
            current_index = 0;
            if(playback.size()>0) {
                std::cout << "acidcam: Loaded AutoFilter: [" << filename << "] - " << playback.size() << " items.\n";
            }
            return true;
        }

    private:
        std::vector<AutoFilterIndex> playback;
        int current_index;

        void parseLine(int line_index, const std::string &line) {
            auto pos = line.find(":");
            if(pos == std::string::npos) {
                std::cerr << "acidcam: AutoFilter file Error missing : Line [" << line_index << "]\n";
                return;
            }
            std::string shader = line.substr(0, pos);
            std::string right = line.substr(pos+1, line.length()-pos);
            pos = right.find(":");
            if(pos == std::string::npos) {
                std::cerr << "acidcam: AutoFilter missing second : Line [" << line_index << "]\n";
                return;
            }
            std::string filter;
            filter = right.substr(0, pos);
            right = right.substr(pos+1, right.length()-pos);
            int filter_count = atoi(right.c_str());
            if(filter_count <= 0) {
                std::cerr << "acidcam: AutoFilter Invalid filter count... Line [" << line_index << "]\n";
                return;
            }
            playback.push_back(AutoFilterIndex(shader,filter,filter_count));
        }
    };

}


#endif
