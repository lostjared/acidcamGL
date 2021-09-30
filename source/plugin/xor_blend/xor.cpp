#include"ac.h"
#include<cstdlib>
#include<ctime>

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    int col_size = collection.size()-1;
    collection.shiftFrames(frame);
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            long val[3] = {0};
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            for(int q = 0; q < col_size; ++q) {
                cv::Mat &m = collection.frames[q];
                cv::Vec3b pix = m.at<cv::Vec3b>(z, i);
                val[0] += pix[0];
                val[1] += pix[1];
                val[2] += pix[2];
            }
            for(int j = 0; j < 3; ++j)
                    pixel[j] = static_cast<unsigned char>(0.5 * pixel[j]) ^ static_cast<unsigned char>(0.5 * val[j]);
        }
    }
}
