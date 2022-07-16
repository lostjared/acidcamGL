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
    
    static int init = 0;
    if(init == 0) {
        ac::init();
        init = 1;
        srand(static_cast<unsigned int>(time(0)));
    }
    
    static int offset1 = rand()%(ac::solo_filter.size()-1);
    static int offset2 = rand()%(ac::solo_filter.size()-1);
    static double alpha = 0.1;
    
    cv::Mat cpy = frame.clone();
    ac::CallFilter(ac::solo_filter[offset1], frame);
    ac::CallFilter(ac::solo_filter[offset2], cpy);
    alphablend(frame, cpy, alpha);
    static int dir = 1;
    static int one = 1;
    if(dir == 1) {
        alpha += 0.05;
        if(alpha >= 1) {
            alpha = 1;
            dir = 0;
        }
    } else {
        alpha -= 0.05;
        if(alpha <= 0.1) {
            alpha = 0.1;
            dir = 1;
            if(one == 1) {
                offset1 = rand()%(ac::solo_filter.size()-1);
                one = 0;
            } else {
                offset2 = rand()%(ac::solo_filter.size()-1);
                one = 1;
            }
        }
    }
}
