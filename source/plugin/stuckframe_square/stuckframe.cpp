#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    static int off = 0;
    static int array[] = { 1, 2, 2, 3, 3, 3 };
    static int array_off = 0;
    static int off_on = 1;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &f = collection.frames[off];
            cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q) {
                pixel[q] = static_cast<unsigned char>(pixel[q] * 0.5 + pix[q] * 0.5);
            }
                
            if(i%20 == 0 && z%20 == 0 && off_on == 1) {
                off_on = 0;
                off += array[array_off];
                if(off > MAX - 1) {
                    off = 0;
                }
            }
        }
        if(z%40 == 0) {
            off_on = 1;
        }
    }
    array_off++;
    if(array_off > 5)
        array_off = 0;
}
