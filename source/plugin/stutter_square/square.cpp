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
            if((rand()%8)==0) {
                stored = frame.clone();
                on = !on;
            }
            frame = stored.clone();
        } else {
            if((rand()%5) == 0)
                on = !on;
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<32> collection;
    if(collection.empty())
        srand(static_cast<unsigned int>(time(0)));
    
    stutter_filter(frame);
    collection.shiftFrames(frame);
    int square_size = 4+(rand()%28);
    static int offset = 0;
    for(int z = 0; z < frame.rows-square_size; z += square_size) {
        for(int i = 0; i < frame.cols-square_size; i += square_size) {
            for(int x = 0; x+i < frame.cols && x < square_size; ++x) {
                for(int y = 0; z+y < frame.rows && y < square_size; ++y) {
                    cv::Vec3b &pixel = ac::pixelAt(frame,z+y, i+x);
                    cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z+y, i+x);
                    pixel = pix;
                }
            }
            ++offset;
            if(offset > (collection.size()-1))
                offset = 0;
        }
    }
}
