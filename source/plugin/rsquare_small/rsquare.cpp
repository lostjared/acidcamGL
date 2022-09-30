#include"ac.h"

void drawSquare(cv::Mat &frame, int x, int y, int w, int h, cv::Mat &off) {
    for(int i = x; i < x+w && i < frame.cols; ++i) {
        for(int z = y; z < y+h && z < frame.rows; ++z) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            pixel = off.at<cv::Vec3b>(z, i);
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    } else collection.shiftFrames(frame);
    int num_squares = 300+(rand()%100);
    for(int i = 0; i < num_squares; ++i) {
        int x = rand()%frame.cols;
        int y = rand()%frame.rows;
        int w = 10+rand()%((frame.cols/8)-10);
        int h = 10+rand()%((frame.rows/8)-10);
        drawSquare(frame, x, y, w, h, collection.frames[rand()%(MAX-1)]);
    }
}
