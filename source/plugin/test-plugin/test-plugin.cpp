#include<iostream>
#include"plugin-program.hpp"
#include<unistd.h>

int main(int argc, char **argv) {
    
    if(argc != 2) {
        std::cerr << "Error: requires one arguemnt of plugin to test...\n";
        exit(EXIT_FAILURE);
    }
    
    if(argc == 2) {
        acidcam::AC_Program program;
        if(program.load(argv[1])) {
            std::cout << "plugin: loaded..\n";
        }
        cv::Mat m = cv::Mat::zeros(480, 640, CV_8UC3);
        cv::namedWindow("test-plugin");
        for(int i = 0; i < 5; ++i) {
            program.exec(m);
            cv::imshow("test-plugin", m);
        }
        cv::waitKey(1000);
        sleep(4);
        std::cout << "plugin: tests passed...\n";
        exit(0);
    }
    return 0;
}
