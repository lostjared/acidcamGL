#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static float rgb[3] = { 1,2,3 };
    static float inc[3] = { 0.1, 0.2, 0.3 };
    static float max = 7.0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = static_cast<unsigned char>((0.5 * pixel[j]) + (0.5 * pixel[j] * rgb[j]));
            }
        }
    }
   
    static int dir = 1;
    static int dir2 = 1;
    if(dir == 1) {
        rgb[0] += inc[0];
        rgb[1] += inc[1];
        rgb[2] += inc[2];
        if(rgb[0] > max) {
            dir = 0;
            if(dir2 == 1) {
                max += 0.5f;
                if(max > 15)
                    dir2 = 0;
            } else {
                max -= 0.5f;
                if(max <= 7.0)
                   dir2 = 1;
                   
            }
            inc[0] += 0.1;
            inc[1] += 0.2;
            inc[2] += 0.3;
        }
    } else {
        rgb[0] -= inc[0];
        rgb[1] -= inc[1];
        rgb[2] -= inc[2];
    
        if(rgb[0] <= 1.0) {
            dir = 1;
            if(inc[0] > 3.0) {
                inc[0] = 0.1;
                inc[1] = 0.2;
                inc[2] = 0.3;
            }
        }
    }
    
    
}
