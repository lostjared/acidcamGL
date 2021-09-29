// This one runs really slow
#include"ac.h"
#include<cstdlib>
#include<ctime>

void stutter_filter(cv::Mat  &frame) {
    static cv::Mat stored;
    static cv::Size stored_size;
    
    if(stored_size != frame.size()) {
        srand(static_cast<int>(time(0)));
        stored = frame.clone();
        stored_size = frame.size();
    } else {
        if(stored.empty())
            stored = frame.clone();

        static bool on = true;
        if(on == true) {
            if((rand()%15)==0) {
                stored = frame.clone();
                on = !on;
            }
            frame = stored.clone();
        } else {
            if((rand()%10) == 0)
                on = !on;
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<32> collection;
    if(collection.empty())
        srand(static_cast<unsigned int>(time(0)));
    
    collection.shiftFrames(frame);
    stutter_filter(frame);
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
                        for(int j = 0; j < 3; ++j) {
                            pixel[j] = static_cast<unsigned char>((0.5 * pixel[j]) + (0.5 * pix[j]));
                        }
                    }
                }
            }
        }
        if(dir == 1) {
            ++index;
            if(index > (collection.size()-1)) {
                index = collection.size()-1;
                dir = 0;
            }
        } else {
            --index;
            if(index <= 0) {
                index = 0;
                dir = 1;
            }
        }
    }
    if(square_dir == 1) {
        square_size += 2;
        if(square_size >= 64) {
            square_size = 64;
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
