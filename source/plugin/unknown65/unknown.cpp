#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    int off_x = rand()%frame.cols;
    for(int z = 0; z < frame.rows; z += 5) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            
            int xpos = AC_GetFZ(frame.rows, z, off_x);
            if(xpos >= 0 && xpos < frame.cols) {
                cv::Vec3b &pix = frame.at<cv::Vec3b>(z, xpos);
                for(int q = 0; q < 3; ++q) {
                    pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
                }
            }
            
        }
        if((z%2) == 0) {
            --off_x;
            if(off_x <= 1)
                off_x = rand()%frame.cols;
        }
    }
    
    
}
