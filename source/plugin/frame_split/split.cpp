#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    }
    
    
    static int timeout = 0;
    static int wait = rand()%15+5;
    
    if(++timeout > wait) {
        timeout = 0;
        wait = rand()%15+5;
    } else return;
    
    collection.shiftFrames(frame);
    
    auto drawShift = [&](cv::Mat &frame_copy, int y, int h) {
        for(int z = y; z < y+h && z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b &pix = frame_copy.at<cv::Vec3b>(z, i);
                pixel = pix;
            }
        }
    };

    int row = rand()%(frame.rows);
    int row_h = rand()%(frame.rows);
    drawShift(collection.frames[7], row, row_h);
}
