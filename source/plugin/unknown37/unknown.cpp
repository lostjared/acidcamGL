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
    
    static bool flash = true;
    if(flash) {
        static int dir = 1;
        static int offset = 0;
        static int size_y = frame.rows/16;
        static int size_x = 2;
        static int dir2 = 1;
        static int direction = 1;
        
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                int cy = AC_GetFZ(frame.rows-1, i, size_y);
                int cx = AC_GetFX(frame.cols-1, z, size_x);
                
                if(cy >= 0 && cy < frame.rows && cx >= 0 && cx < frame.cols) {
                    cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(cy, cx);
                    pixel = pix;
                }
            }
            
            if(dir == 1) {
                size_y += rand()%10;
                if(size_y > frame.rows*2) {
                    dir = 0;
                    size_y = frame.rows*2;
                }
            } else {
                size_y -= rand()%10;
                if(size_y <= 2) {
                    dir = 1;
                    size_y = 2;
                }
            }
            
            if(dir2 == 1) {
                size_x += rand()%10;
                if(size_x > frame.rows*4) {
                    dir2 = 0;
                    size_x = frame.rows*4;
                }
            } else {
                size_x -= rand()%10;
                if(size_x <= 2) {
                    dir2 = 1;
                    size_x = 2;
                }
            }
        }
        
        if(direction == 1) {
            if(++offset > (MAX-1)) {
                direction = 0;
                offset = MAX-1;
            }
        } else {
            if(--offset <= 0) {
                direction = 1;
                offset = 0;
            }
        }
        
    } else {
        static int dir = 1;
        static int offset = 0;
        static int div = 2;
        static int size_y = frame.rows/16;
        
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                int cy = AC_GetFZ(frame.rows-1, z, size_y);
                if(cy >= 0 && cy < frame.rows && i >= 0 && i < frame.cols) {
                    cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(cy, i);
                    pixel = pix;
                }
            }
            size_y ++;
            if(size_y > frame.rows*2)
                size_y = frame.rows/16;
        }
        
        if(++offset > (MAX-1)) {
            offset = 0;
        }
    }
    
    flash = (flash == true) ? false : true;
}
