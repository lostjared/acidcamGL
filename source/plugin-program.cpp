#include "plugin-program.hpp"
#ifndef _WIN32
#include<dlfcn.h>
namespace acidcam {
    
    
    AC_Program::AC_Program() {
        library = NULL;
        addr = NULL;
    }
    
    AC_Program::~AC_Program() {
       // if(library != NULL)
       //     dlclose(library);
    }
    bool AC_Program::load(const std::string &text) {
       library = dlopen(text.c_str(), RTLD_LAZY);
        if(library == NULL) {
            std::cerr << "acidcam: Error could not open: " << text << "\n";
            return false;
        }
        void *addr;
        // load the plugin function to process pixels
        addr = dlsym(library, "filter");
        
        pixf = (pixel)addr;
        const char *error;
        error = dlerror();
        if(error) {
            std::cout << "acidcam: " << error << "...\n";
            return false;
        }
        return true;
    }
    void AC_Program::exec(cv::Mat &frame) {
        pixf(frame);
    }
    
}
#endif
