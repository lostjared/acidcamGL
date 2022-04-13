#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    int r = 3+rand()%7;
    for(int i = 0; i < r; ++i)
        ac::MedianBlur(frame);
    
    collection.shiftFrames(frame);
    static double alpha = 1.0, alpha_max = 3.0;
   
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Scalar value;
                for(int j = 0; j < collection.size(); ++j) {
                    cv::Vec3b pixel = collection.frames[j].at<cv::Vec3b>(z, i);
                    for(int q = 0; q < 3; ++q) {
                        value[q] -= pixel[q];
                    }
                }
                cv::Vec3b &pixel = ac::pixelAt(*frame,z, i);
                for(int j = 0; j < 3; ++j) {
                    int val = 1+static_cast<int>(value[j]);
                    pixel[j] = static_cast<unsigned char>(pixel[j] ^ val);
                }
            }
        }
    };
    ac::UseMultipleThreads(frame, 16, callback);
//    cv::Mat copyf = frame.clone();
    ac::setGamma(frame, frame, 4);
}
