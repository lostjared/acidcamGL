#include"ac.h"

void drawSquare(cv::Mat &frame, int x, int y, int w, int h, cv::Mat &off) {
    for(int i = x; i < w; ++i) {
        for(int z = y; z < h; ++z) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            pixel = off.at<cv::Vec3b>(z, i);
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    for(int i = 0; i < (50+rand()%50); ++i) {
        int x = rand()%frame.cols;
        int y = rand()%frame.rows;
        int w = 10+rand()%(frame.cols-10);
        int h = 10+rand()%(frame.rows-10);
        drawSquare(frame, x, y, w, h, collection.frames[rand()%(MAX-1)]);
    }
}
