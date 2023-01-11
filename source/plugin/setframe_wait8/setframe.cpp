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
    
    static int wait = rand()%8;
    static int count = 0;
    static int size_x = 8;
    static int num = 2;
    static int offset = 0;
    static double alpha = 1.0;
    static int skip = 2;
    
    if(++count > wait) {
        count = 0;
        wait = rand()%24;
        for(int z = 0; z < frame.rows; z += size_x) {
            for(int i = 0; i < frame.cols; i += size_x) {
                if(rand()%num == 0) {
                    for(int x = 0; x < size_x && x+i < frame.cols; ++x) {
                        for(int y = 0; y < size_x && y+z < frame.rows; ++y) {
                            
                            if(rand()%skip == 0) continue;
                            
                            cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z+y, i+x);
                            
                            cv::Vec3b &pixel = ac::pixelAt(frame, z+y, i+x);
                            for(int q = 0; q < 3; ++q)
                            pixel[q] = ac::wrap_cast( pixel[q]+(alpha*pix[q]));
                            
                            alpha += 0.05;
                            if(alpha > 5)
                                alpha = 1.0;
                        }
                    }
                }
            }
        }
    }
    
    if(++skip > 5)
        skip = 2;
    
    static int dir = 1;
    if(dir == 1) {
        if(++size_x >= 32) {
            dir = 0;
        }
    } else {
        if(--size_x <= 8) {
            dir = 1;
        }
    }
    static int dir2 = 1;
    if(dir2 == 1) {
        if(++offset > (MAX-1)) {
            offset = rand()%(MAX-1);
            ++num;
            if(num > 16)
                num = 1+(rand()%16);
            dir2 = 0;
       
        }
    } else {
        if(--offset <= 1) {
            offset = rand()%(MAX-1);
            ++num;
            if(num > 8)
                num = 1+(rand()%8);
            dir2 = 1;
        }
    }
}
