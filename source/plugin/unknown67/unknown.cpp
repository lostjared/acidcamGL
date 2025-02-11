#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
    collection.shiftFrames(frame);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b &pix = collection.frames[rand()%(MAX-1)].at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q) {
                if(std::abs(pixel[q]-pix[q]) > 25) {
                    if((rand()%2) == 0)
                        pixel[q] ^= ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
                }
            }
        }
    }
}
