#include"ac.h"

void alphablend(cv::Mat &src, cv::Mat &cpy, double alpha) {
    for(int z = 0; z < src.rows; ++z) {
        for(int i = 0; i < src.cols; ++i) {
            cv::Vec3b &pixel = src.at<cv::Vec3b>(z, i);
            cv::Vec3b &pix = cpy.at<cv::Vec3b>(z, i);
            for(int j = 0; j < 3; ++j) {
                pixel[j] = cv::saturate_cast<unsigned char>((alpha * pixel[j]) + ( (1-alpha) * pix[j]));
            }
        }
    }
}


extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    else if(rand()%3==0)
        collection.shiftFrames(frame);
    
    static double alpha = 0.1;
    alphablend(frame, collection.frames[3], alpha);
    alpha += 0.05;
    if(alpha > 1.0)
        alpha = 0.1;
    
}
