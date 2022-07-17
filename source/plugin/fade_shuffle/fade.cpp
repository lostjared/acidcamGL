#include"ac.h"
#include<cstdlib>
#include<ctime>


void alphablend(cv::Mat &src, cv::Mat &cpy, double alpha) {
    for(int z = 0; z < src.rows; ++z) {
        for(int i = 0; i < src.cols; ++i) {
            cv::Vec3b &pixel = src.at<cv::Vec3b>(z, i);
            cv::Vec3b &pix = cpy.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = ac::wrap_cast((alpha * pixel[j]) + ( 1-alpha * pix[j]));
            }
        }
    }
}


extern "C" void filter(cv::Mat  &frame) {
    
    static std::vector<std::string> fname;
    static int init = 0;
    if(init == 0) {
        ac::init();
        init = 1;
        srand(static_cast<unsigned int>(time(0)));
        std::copy(ac::solo_filter.begin(), ac::solo_filter.end(), std::back_inserter(fname));
        unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
             std::shuffle(fname.begin(), fname.end(), std::default_random_engine(seed));
    }
    
    static int offset1 = 0;
    static int offset2 = 1;
    static double alpha = 0.1;
    
    cv::Mat cpy = frame.clone();
    ac::CallFilter(fname[offset1], frame);
    ac::CallFilter(fname[offset2], cpy);
    alphablend(frame, cpy, alpha);
    static int dir = 1;
    if(dir == 1) {
        alpha += 0.05;
        if(alpha >= 1) {
            alpha = 1;
            dir = 0;
            offset2++;
            if(offset2 > (fname.size()-1)) {
                offset2 = 0;
                unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
                     std::shuffle(fname.begin(), fname.end(), std::default_random_engine(seed));
            }
            
        }
    } else {
        alpha -= 0.05;
        if(alpha <= 0.1) {
            alpha = 0.1;
            dir = 1;
            offset1++;
            if(offset1 > (fname.size()-1)) {
                offset1 = 0;
                unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
                     std::shuffle(fname.begin(), fname.end(), std::default_random_engine(seed));
            }
            
        }
    }
}
