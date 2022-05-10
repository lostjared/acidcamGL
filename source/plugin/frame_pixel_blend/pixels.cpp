#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else
        collection.shiftFrames(frame);
    
    static int off = 0;

    cv::Vec3b pix;
    static int offset_x = 0;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            pix = frame.at<cv::Vec3b>(z, offset_x);
            pixel[0] = ac::wrap_cast((0.5 * pixel[0]) + (0.5 * pix[0]));
            pixel[1] = ac::wrap_cast((0.5 * pixel[1]) + (0.5 * pix[1]));
            pixel[2] = ac::wrap_cast((0.5 * pixel[2]) + (0.5 * pix[2]));
        }
        offset_x++;
        if(offset_x > frame.cols-1) offset_x = 0;
        
    }
}
