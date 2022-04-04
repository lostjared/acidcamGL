#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    
    static int init = 0;
    if(init == 0) {
        srand(static_cast<unsigned int>(time(0)));
        init = 1;
    }
    
    cv::Mat f = frame.clone();
    static int r_pos_x = rand()%(frame.cols-1);
    static int r_pos_y = rand()%(frame.rows-1);
    static int start_pos_x = r_pos_x;
    static int start_pos_y = r_pos_y;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = f.at<cv::Vec3b>(start_pos_y, start_pos_x);
            pixel = pix;
            ++start_pos_x;
            if(start_pos_x > frame.cols-2)
                start_pos_x = 0;
        }
        ++start_pos_y;
        if(start_pos_y > frame.rows-2)
            start_pos_y = 0;
    }
}
