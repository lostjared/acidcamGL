#include "ac.h"

unsigned char wrap_cast(double d) {
    unsigned long x = static_cast<unsigned long>(d);
    if(x > 255)
        x = x%255;
    return static_cast<unsigned char>(x);
}

extern "C" void filter(cv::Mat &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    static int rsize = 150;
    int rsize_c = 0;
    int rsize_count = rand()%rsize;
    static int offset = 0;
    int rsize_x = rand()%rsize;
    int rgb = rand()%2;
    
    for(int z = 0; z < frame.rows; ++z) {
        int offset_x = 0;
        for(int i = rsize_x; i < frame.cols; ++i) {
            cv::Mat &m = collection.frames[offset];
            cv::Mat &m2 = collection.frames[7];
            cv::Vec3b &pixel_diff = m2.at<cv::Vec3b>(z, i);
            cv::Vec3b &pixel_off = m.at<cv::Vec3b>(z, offset_x);
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            bool add = false;
            for(int j = 0; j < 3; ++j) {
                if(abs(pixel[j]-pixel_diff[j]) > 15) {
                    add = true;
                    break;
                }
            }
            if(add == true)
                pixel[rgb] = wrap_cast((0.5 * pixel[rgb]) + (0.5 * pixel_off[rgb]));
            
            offset_x++;
        }
        ++rsize_c;
        if(rsize_c > rsize_count) {
            rsize = 50+rand()%50;
            rsize_x = rand()%rsize;
            rsize_c = 0;
            rsize_count = 25+rand()%rsize;
            offset++;
            if(offset > (collection.size()-1)) {
                offset = 0;
            }
            rgb++;
            if(rgb > 2)
                rgb = 0;
        }
    }
}
