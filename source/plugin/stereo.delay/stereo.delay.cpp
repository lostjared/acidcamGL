#include"ac.h"
#include<cstdlib>
#include<ctime>

extern "C" void filter(cv::Mat  &frame) {
/*
    cv::Mat img;
    ac::ac_resize(frame, img, cv::Size(frame.cols/2, frame.rows));
    static ac::MatrixCollection<5> collection;
    collection.shiftFrames(img);
    cv::Mat &copy = collection.frames[4];
    for(int z = 0; z < frame.rows; ++z) {
        int x = 0;
        for(int i = frame.cols/2+10; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = img.at<cv::Vec3b>(z, x);
            ++x;
            pixel = pix;
        }
        for(int i = 0; i < frame.cols/2; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = copy.at<cv::Vec3b>(z, i);
            pixel = pix;
        }
        for(int i = frame.cols/2; i < frame.cols/2+10; ++i) {
            frame.at<cv::Vec3b>(z, i) = cv::Vec3b(0, 0, 0);
        }
    }
*/
}
