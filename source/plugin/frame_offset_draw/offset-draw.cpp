#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    
    static char offset = 0xF;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            if(z+1 < frame.rows && i+1 < frame.cols) {
                cv::Vec3b pix[4];
                pix[0] = collection.frames[1].at<cv::Vec3b>(z+1, i);
                pix[1] = collection.frames[3].at<cv::Vec3b>(z, i+1);
                pix[2] = collection.frames[7].at<cv::Vec3b>(z+1, i+1);
             
                for(int j = 0; j < 3; ++j)
                    if(abs(pixel[j]-pix[j][j]) > 25)
                        pixel[j] += pix[j][j] ^ offset;
            }
        }
    }
    if(++offset > 120) {
        offset = 0xF;
    }
}
