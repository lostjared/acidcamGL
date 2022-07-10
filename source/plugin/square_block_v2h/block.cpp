#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<32> collection;
    collection.shiftFrames(frame);
    static int square_size = 4, square_dir = 1;
    static int index = 0;
    static int dir = 1;
    
    for(int z = 0; z < frame.rows; z += square_size) {
        for(int i = 0; i < frame.cols; i += square_size) {
            for(int y = 0; y < square_size; ++y) {
                for(int x = 0; x < square_size; ++x) {
                    if(z+y < (frame.rows-1) && i+x < (frame.cols-1)) {
                        cv::Vec3b &pixel = ac::pixelAt(frame,z+y, i+x);
                        cv::Vec3b pix = collection.frames[index].at<cv::Vec3b>(z+y, i+x);
                        for(int q = 0; q < 3; ++q)
                      //  pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
                            pixel = pix;
                    }
                }
            }
        }

        if(dir == 1) {
            ++index;
            if(index > (collection.size()-1)) {
                index = rand()%(collection.size()-1);
                dir = 0;
            }
        } else {
            --index;
            if(index <= 0) {
                index = rand()%(collection.size()-1);
                dir = 1;
            }
        }
    }
    
    if(square_dir == 1) {
        square_size += 2;
        if(square_size >= 64) {
            square_size = 8;
            square_dir = 0;
        }
    } else {
        square_size -= 2;
        if(square_size <= 2) {
            square_size = 2;
            square_dir = 1;
        }
    }
}

