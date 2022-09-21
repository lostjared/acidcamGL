#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int lazy_init = 1;
    if(lazy_init == 1) {
        lazy_init = 0;
        srand(static_cast<unsigned int>(time(0)));
    }
    static int timeout = 30+rand()%30;
    static int wait = 0;
    if(++wait > timeout) {
        wait = 0;
        timeout = 30+rand()%30;
        cv::Mat frame_copy = frame.clone();
        int rand_start_x = rand()%(frame.cols/8);
        int x = 0;
        
        for(int i = rand_start_x; i < frame.cols; ++i) {
            for(int z = 0; z < frame.rows/2; ++z) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b &cpix = frame_copy.at<cv::Vec3b>(z, x);
                for(int q = 0; q < 3; ++q) {
                    pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * cpix[q]));
                }
            }
            x ++;
        }
        
        int rand_start_x2 = rand()%(frame.cols/8);
        int x2 = 0;
        
        for(int i = rand_start_x2; i < frame.cols; ++i) {
            for(int z = frame.rows/2; z < frame.rows; ++z) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b &cpix = frame_copy.at<cv::Vec3b>(z, x2);
                for(int q = 0; q < 3; ++q) {
                    pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * cpix[q]));
                }
            }
            x2 ++;
        }
    }
}
