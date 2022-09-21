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
        int rand_start_y = rand()%(frame.rows/8);
        int y = 0;
        
        for(int z = rand_start_y; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols/2; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b &cpix = frame_copy.at<cv::Vec3b>(y, i);
                for(int q = 0; q < 3; ++q) {
                    pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * cpix[q]));
                }
            }
            ++y;
        }
        int rand_start_y2 = rand()%(frame.rows/8);
        int y2 = 0;
        for(int z = rand_start_y2; z < frame.rows; ++z) {
            for(int i = frame.cols/2; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b &cpix = frame_copy.at<cv::Vec3b>(y2, i);
                for(int q = 0; q < 3; ++q) {
                    pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * cpix[q]));
                }
            }
            ++y2;
        }
    }
}
