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
    
    static int start_x[2] = {0};
    static int start_y[2] = {0};
    static int offset = 0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int xpos = start_x[0]+i;
            if(xpos >= 0 && xpos < frame.cols) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, xpos);
                pixel[0] = pix[0];
            }
            int xpos2 = i-start_x[1];
            if(xpos2 >= 0 && xpos2 < frame.cols) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, xpos2);
                pixel[1] = pix[1];
            }
            
            int ypos = start_y[0]+z;
            if(ypos >= 0 && ypos < frame.rows) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(ypos, i);
                pixel[2] = pix[2];
            }
            int ypos2 = z-start_y[1];
            if(ypos2 >= 0 && ypos2 < frame.rows) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(ypos2, i);
                pixel[0] = pix[0];
            }
        }
    }
    
    start_x[0] += 50;
    if(start_x[0] > frame.cols) {
        start_x[0] = 0;
    }
    start_x[1] -= 50;
    if(start_x[1] <= 0) {
        start_x[1] = frame.cols;
    }

    
    start_y[0] += 50;
    if(start_y[0] > frame.rows) {
        start_y[0] = 0;
    }
    start_y[1] -= 50;
    if(start_y[1] <= 0) {
        start_y[1] = frame.rows;
    }
    
    if(++offset > (MAX-1))
        offset = 0;
}
