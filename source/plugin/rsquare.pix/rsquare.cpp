#include"ac.h"


void drawSquare(cv::Mat &frame, int x, int y, int w, int h, cv::Mat &off) {
    for(int i = x; i < x+w && i < frame.cols; ++i) {
        for(int z = y; z < y+h && z < frame.rows; ++z) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b &pix = off.at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q)
                pixel[q] = ac::wrap_cast((0.5 * pixel[q]+(0.5 * pix[q])));
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    collection.shiftFrames(frame);
    for(int i = 0; i < ((frame.rows * frame.cols)/32)/32; ++i) {
        int x = rand()%frame.cols;
        int y = rand()%frame.rows;
        int w = rand()%32;
        int h = rand()%32;
        drawSquare(frame, x, y, w, h, collection.frames[rand()%(MAX-1)]);
    }
}
