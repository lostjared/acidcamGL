#include"ac.h"
#include<cstdlib>
#include<ctime>

unsigned char wrap_cast(double d) {
    unsigned long x = static_cast<unsigned long>(d);
    if(x > 255)
        x = x%255;
    return static_cast<unsigned char>(x);
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    static int line_y[] = {
        rand()%(frame.rows-125),rand()%(frame.rows-125),rand()%(frame.rows-125),rand()%(frame.rows-125),rand()%(frame.rows-125),rand()%(frame.rows-125), 0};
    static int line_h[] = {25+rand()%100,25+rand()%100,25+rand()%100,25+rand()%100,25+rand()%100,25+rand()%100, 0};
    
    static int off = 0;
    for(int q = 0; q < 5; ++q) {
        for(int y = line_y[q]; y <  line_y[q]+line_h[q] && y < frame.rows; ++y) {
            for(int x = 0; x < frame.cols; ++x) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
                cv::Mat &m = collection.frames[off];
                cv::Vec3b &col = m.at<cv::Vec3b>(y, x);
                for(int j = 0; j < 3; ++j) {
                    pixel[j] = wrap_cast((0.5 * pixel[j]) + (0.5 * col[j]));
                }
            }
        }
    }
    
    ++off;
    if(off >= MAX-1)
        off = 0;
    
    for(int q = 0; q < 5; ++q) {
        line_y[q] -= 10;
        if(line_y[q] <= 1) {
            line_y[q] = rand()%(frame.rows-125);
            line_h[q] = 25+rand()%100;
        }
    }
}
