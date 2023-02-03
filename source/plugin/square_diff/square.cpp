#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    }
    else
        collection.shiftFrames(frame);
    
    auto drawSquare = [&](cv::Mat sframe, int x, int y, int w, int h) {
        for(int i = x; i < x+w && i < frame.cols; ++i) {
            for(int z = y; z < y+h && z < frame.rows; ++z) {
                if(i >= 0 && i < frame.cols && z >= 0 && z < frame.cols) {
                    cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                    cv::Vec3b &pix = sframe.at<cv::Vec3b>(z, i);
                    pixel = pix;
                }
            }
        }
    };
    
    int square_h = 16;
    int square_w = 64;
    static int offset = 0;
    
    for(int z = 0; z < frame.rows; z += square_h) {
        square_w = 1+rand()%32;
        for(int i = 0; i < frame.cols; i += square_w) {
            drawSquare(collection.frames[offset], i, z, square_w, square_h);
        }
        if(++offset > (MAX-1)) {
            offset = rand()%(MAX-1);
        }
        square_h = 1+rand()%128;
    }
    
}
