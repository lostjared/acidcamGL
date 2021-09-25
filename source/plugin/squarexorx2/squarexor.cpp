#include"ac.h"
#include<cstdlib>
#include<ctime>

extern "C" void filter(cv::Mat  &frame) {
    static double alpha = 1.0;
    for(int z = 0; z < frame.rows-2; z += 2) {
        for(int i = 0; i < frame.cols-2; i += 2) {
            if(i+1 < frame.cols && z+1 < frame.rows) {
                unsigned long total[4] = {0};
                cv::Vec3b *colors[] = {
                    &frame.at<cv::Vec3b>(z, i),
                    &frame.at<cv::Vec3b>(z, i+1),
                    &frame.at<cv::Vec3b>(z+1, i),
                    &frame.at<cv::Vec3b>(z+1, i+1)
                };
                for(int q = 0; q < 4; ++q) {
                    for(int j = 0; j < 3; ++j)
                        total[j] += (*colors[q])[j];
                }
                for(int q = 0; q < 4; ++q) {
                    for(int j = 0; j < 3; ++j) {
                        (*colors[q])[j] ^= static_cast<unsigned char>(alpha*total[j]);
                    }
                }
            }
        }
    }
    
    static int dir = 1;
    if(dir == 1) {
        alpha += 0.07;
        if(alpha > 4.0)
            dir = 0;
    } else {
        alpha -= 0.07;
        if(alpha <= 1.0)
            dir = 1;
    }
}
