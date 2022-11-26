#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else if(rand()%2 == 0)
        collection.shiftFrames(frame);
    
    static int flash = 0;
    switch(flash) {
        case 0: {
            static int dir = 1;
            static int offset = 0;
            static int div = 2;
            static int size_x = frame.cols/16;
            
            for(int z = 0; z < frame.rows; ++z) {
                for(int i = 0; i < frame.cols; ++i) {
                    cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                    int cx = AC_GetFX(frame.cols-1, i, size_x);
                    if(cx >= 0 && cx < frame.cols && z >= 0 && z < frame.rows) {
                        cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, cx);
                        pixel = pix;
                    }
                }
                size_x ++;
                if(size_x > frame.cols*2)
                    size_x = frame.cols/16;
            }
            
            if(++offset > (MAX-1)) {
                offset = 0;
            }
        }
            break;
        case 1: {
            int width = 1+(rand()%frame.cols);
            int skip = 0;
            static int offset = 0;
            
            for(int z = 0; z < frame.rows; ++z) {
                for(int i = 0; i < frame.cols; ++i) {
                    cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
                    
                    int offx = AC_GetFX(frame.cols-1, i, width);
                    
                    cv::Vec3b &cpix = collection.frames[offset].at<cv::Vec3b>(z, offx);
                
                    pixel = cpix;
                }
                width += 1+(rand()%5);
                if(width%10) {
                    if(++offset > (MAX-1))
                        offset = 0;
                }
                
            }
        }
            break;
        case 2: {
            static int dir = 1;
            static int offset = 0;
            
            frame = collection.frames[offset].clone();
            
            if(dir == 1) {
                static int wait = 0;
                static int timeout = rand()%10;
                if(++offset > (MAX-1)) {
                    offset = 0;
                }
                if(++wait > timeout) {
                    wait = 0;
                    timeout = rand()%10;
                    dir = 0;
                }
            } else {
                static int wait = 0;
                static int timeout = rand()%10;
                if(--offset <= 0) {
                    offset = MAX-1;
                }
                if(++wait > timeout) {
                    wait = 0;
                    timeout = rand()%10;
                    dir = 1;
                }
            }
        }
            break;
    }
    
    if(++flash > 2)
        flash = 0;
    
}
