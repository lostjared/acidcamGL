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
            if((rand()%3)==0) {
                stored = frame.clone();
                on = !on;
            }
            frame = stored.clone();
        } else {
            if((rand()%4) == 0)
                on = !on;
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<4> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    stutter_filter(frame);
    collection.shiftFrames(frame);
    int square_size_x = 4+(rand()%8);
    int square_size_y = 4+(rand()%8);
    static int offset = 0;
    for(int z = 0; z < frame.rows-square_size_y; z += square_size_y) {
        for(int i = 0; i < frame.cols-square_size_x; i += square_size_x) {
            for(int x = 0; x+i < frame.cols && x < square_size_x; ++x) {
                for(int y = 0; z+y < frame.rows && y < square_size_y; ++y) {
                    cv::Vec3b &pixel = ac::pixelAt(frame,z+y, i+x);
                    cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z+y, i+x);
                    pixel = pix;
                }
            }
            
            if((rand()%8) == 0) {
                ++offset;
                if(offset > (collection.size()-1))
                    offset = 0;
            }
        }
       
    }
}
