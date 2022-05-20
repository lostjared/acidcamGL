#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    cv::Vec3b pix;
    static int offset_y = 0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            pix = frame.at<cv::Vec3b>(z, offset_y);
            
            pixel[0] = (pixel[2]/2) * offset_y;
            pixel[1] = (pixel[1]/3) * offset_y;
            pixel[2] = (pixel[0]/4) * offset_y;
        }
        offset_y++;
        if(offset_y > frame.rows/2)
            offset_y = 0;
    }

}
