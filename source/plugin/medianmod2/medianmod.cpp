#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }


   int r = 3+rand()%4;
   for(int j = 0; j < r; ++j)
        ac::MedianBlur(frame);
   
    static int counter = 0;
    
    if(++counter%8==0)
        collection.shiftFrames(frame);
    static int rand_value1 = rand()%255,
    rand_value2 = rand()%255, rand_value3 = rand()%255;
    
    static int off = 0;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b value = m.at<cv::Vec3b>(z, i);
            
            if(value[0] > 50 && pixel[0] > rand_value1) {
                pixel[0] = pixel[0] ^ value[0];
            }
            if(value[1] > 150 && pixel[1] < rand_value2) {
                pixel[1] = pixel[1] ^ value[1];
            }
            if(value[2] < 200 && pixel[2] < rand_value3) {
                pixel[2] = pixel[2] ^ value[2];
            }
        }
    }
    
    static int dir = 1;
    
    if(dir == 1) {
        ++off;
        if(off > MAX-2) {
            dir = 0;
            rand_value1 = rand()%255,
            rand_value2 = rand()%255, rand_value3 = rand()%255;
        }
    } else {
       --off;
        if(off == 0) {
           dir = 1;
            rand_value1 = rand()%255,
            rand_value2 = rand()%255, rand_value3 = rand()%255;
        }
    }
}
