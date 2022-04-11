#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 4;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else
        collection.shiftFrames(frame);
        
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
                for(int q = 0; q < collection.size()-1; ++q) {
                    cv::Vec3b pix1, pix2;
                    cv::Mat &m1 = collection.frames[q];
                    pix1 = m1.at<cv::Vec3b>(m1.rows-z-1, i);
                    pix2 = m1.at<cv::Vec3b>(z, m1.cols-i-1);
                    pixel[0] = ac::wrap_cast((0.5 * pixel[0]) + (0.2 * pix1[0]) + (0.2 * pix2[0]));
                    pixel[1] = ac::wrap_cast((0.5 * pixel[1]) + (0.2 * pix1[1]) + (0.2 * pix2[1]));
                    pixel[2] = ac::wrap_cast((0.5 * pixel[2]) + (0.2 * pix1[2]) + (0.2 * pix2[2]));
                }
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
}
