// Goign to attempt to write something
// starting off slow
#include"ac.h"


extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<16> collection;
    collection.shiftFrames(frame);
    int rnd = rand()%16;
    int rnd_op = rand()%3;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &rnd_frame = collection.frames[rnd];
            cv::Vec3b rnd = rnd_frame.at<cv::Vec3b>(z, i);
                if(rand()%13 == 0) {
                    for(int q = 0; q < 3; ++q) {
                        switch(rnd_op) {
                        case 0:
                                pixel[q] = pixel[q] & rnd[q];
                            break;
                        case 1:
                                pixel[q] = pixel[q] ^ rnd[q];
                            break;
                        case 2:
                                pixel[q] = pixel[q] | rnd[q];
                            break;
                    }
                }
            }
        }
    }
}
