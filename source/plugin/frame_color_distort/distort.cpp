#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static int init = 0;
    if(init == 0) {
        init = 1;
        srand(static_cast<unsigned int>(time(0)));
    }
    
    static int color[3] = {rand()%255, rand()%255, rand()%255};
    
    static int color_stop[3] = {rand()%255, rand()%255, rand()%255 };
    
    static int dir[3] = { rand()%2, rand()%2, rand()%2 };
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            pixel[0] = ac::wrap_cast((0.5 * pixel[0]) + (0.5 * color[0]));
            pixel[1] = ac::wrap_cast((0.5 * pixel[1]) + (0.5 * color[1]));
            pixel[2] = ac::wrap_cast((0.5 * pixel[2]) + (0.5 * color[2]));

        }
    }

    for(int q = 0; q < 3; ++q) {
        if(dir[q] == 1) {
            color[q] += 10;
            if(color[q] >= color_stop[q]) {
                color[q] = 255;
                dir[q] = 0;
                color_stop[q] = rand()%255;
            }
        } else {
            color[q] -= 10;
            if(color[q] <= 0) {
                color[q] = 0;
                dir[q] = 1;
            }
        }
    }
}
