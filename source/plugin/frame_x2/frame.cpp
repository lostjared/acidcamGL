#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 4;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    } else
        collection.shiftFrames(frame);
    
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            
            for(int q = 1; q < 3; ++q) {
                int off_i = AC_GetFX(frame.cols-1, i, frame.cols+(q*(frame.cols/4)));
                cv::Vec3b &pix = collection.frames[q].at<cv::Vec3b>(z, off_i);
            
                for(int j = 0; j < 3; ++j) {
                    pixel[j] = ac::wrap_cast((0.5 * pixel[j]) + (0.5 * pix[j]));
                }
            }
        }
    }
}
