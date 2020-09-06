#ifndef __PLUGIN_PROGRAM__
#define __PLUGIN_PROGRAM__
#ifndef _WIN32
#include<iostream>
#ifdef _WIN32
#include"ac.h"
#else
#include"acidcam/ac.h"
#include<unistd.h>
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
#endif
