#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    static int off = 0;
    int r = 2+rand()%150;
    int counter = 0;
    int rsize = 25+rand()%25;
     for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            if((z%rsize) == 0)
                ++counter;
            if(counter > r) {
                counter = 0;
                r = 2+rand()%150;
                ++off;
                if(off > MAX-1) off = 0;
                rsize = 25+rand()%25;
            }
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &f = collection.frames[off];
            cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
            pixel = pix;
        }
    }
}
