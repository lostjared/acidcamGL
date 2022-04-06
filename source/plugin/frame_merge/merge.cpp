#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(rand()%2 == 0)
        collection.shiftFrames(frame);
    
    static int off = 0;
    cv::Mat &f = collection.frames[off];
    cv::Mat &f2 = collection.frames[3];
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pix = f.at<cv::Vec3b>(z, frame.cols-i-1);
            cv::Vec3b pix2 = f2.at<cv::Vec3b>(z, i);
            pixel[0] = ac::wrap_cast((0.33 * pixel[0]) + (0.33 * pix[0]) + (0.33 * pix2[0]));
            pixel[1] = ac::wrap_cast((0.33 * pixel[1]) + (0.33 * pix[1]) + (0.33 * pix2[1]));
            pixel[2] = ac::wrap_cast((0.33 * pixel[2]) + (0.33 * pix[2]) + (0.33 * pix2[2]));

        }
    }
    static int dir = 1;
    if(dir == 1) {
        if(++off > MAX-1) {
            off = MAX-1;
            dir = 0;
        }
    } else {
        if(--off <= 0) {
            off = 0;
            dir = 1;
        }
    }
}
