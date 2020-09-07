#ifndef __PLUGIN_PROGRAM__
#define __PLUGIN_PROGRAM__
#include<iostream>
#include<string>
#ifndef _WIN32
#include<acidcam/ac.h>
#else
#include"ac.h"
#endif
namespace acidcam {
   
    typedef void (*pixel)(cv::Mat &frame);
    
    class AC_Program {
    public:
        AC_Program();
        ~AC_Program();
        bool load(const std::string &text);
        void exec(cv::Mat &frame);
    private:
        void *library;
        void *addr;
        pixel pixf;
    };
    
}

#endif
