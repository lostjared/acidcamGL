#include<iostream>
#include"plugin-program.hpp"


int main(int argc, char **argv) {
    if(argc == 2) {
        acidcam::AC_Program program;
        if(program.load(argv[1])) {
            std::cout << "plugin: loaded..\n";
        }
        cv::Mat m = cv::Mat::zeros(480, 640, CV_8UC3);
        for(int i = 0; i < 5; ++i) {
            program.exec(m);
        }
        std::cout << "plugin: tests passed...\n";
        exit(0);
    }
    return 0;
}
