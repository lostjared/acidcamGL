#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
    }
    
    static int counter = 0;
    if(++counter > 2) {
        counter = 0;
        collection.shiftFrames(frame);
    }
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b *off[3];
            off[0] = &collection.frames[1].at<cv::Vec3b>(z, i);
            off[1] = &collection.frames[3].at<cv::Vec3b>(z, i);
            off[2] = &collection.frames[7].at<cv::Vec3b>(z, i);
            cv::Vec3b pix = pixel;
            for(int q = 0; q < 3; ++q) {
                pixel[q] = (*off[q])[q];
                pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
            }
        }
    }
}
