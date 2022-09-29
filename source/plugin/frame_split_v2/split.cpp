#include"ac.h"

inline void pixel_blend(cv::Vec3b &pix1, cv::Vec3b &pix2) {
    for(int q = 0; q < 3; ++q) {
        pix1[q] = ac::wrap_cast((0.5 * pix1[q]) + (0.5 * pix2[q]));
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    }
    
    
    static int timeout = 0;
    static int wait = rand()%5+5;
    
    if(++timeout > wait) {
        timeout = 0;
        wait = rand()%5+5;
    } else return;
    
    collection.shiftFrames(frame);
    
    auto drawShift = [&](cv::Mat &frame_copy, int y, int h) {
        for(int z = y; z < y+h && z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b &pix = frame_copy.at<cv::Vec3b>(z, i);
                pixel_blend(pixel, pix);
            }
        }
    };

    int row = rand()%(frame.rows);
    int row_h = rand()%(frame.rows);
    drawShift(collection.frames[7], row, row_h);
}
