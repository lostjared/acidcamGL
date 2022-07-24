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
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    collection.shiftFrames(frame);
    
    static int num_square = 25;
    for(int i = 0; i < num_square; ++i) {
        int x = rand()%frame.cols;
        int y = rand()%frame.rows;
        int w = 25+rand()%(frame.cols-25);
        int h = 25+rand()%(frame.rows-25);
        drawSquare(frame, x, y, w, h, collection.frames[rand()%(MAX-1)]);
    }
    ac::FrameSep2(frame);
    
    static int dir = 1;
    if(dir == 1) {
        num_square += 10;
        if(num_square >= 100) {
            dir = 0;
        }
    } else {
        num_square -= 10;
        if(num_square <= 25) {
            dir = 1;
        }
    }
}
