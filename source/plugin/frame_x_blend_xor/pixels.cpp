#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    cv::Vec3b pix;
    static int offset_x = 0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            pix = frame.at<cv::Vec3b>(z, offset_x);
            pixel[0] = pixel[0]^pix[0];
            pixel[1] = pixel[1]^pix[1];
            pixel[2] = pixel[2]^pix[2];
        }
        offset_x++;
        if(offset_x > frame.cols-1) offset_x = 0;
    }
}
