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
                if(dir == 1) {
                    size_y ++;
                    if(size_y > frame.rows*2)
                        dir = 0;
                } else {
                    size_y --;
                    if(size_y <= 2)
                        dir = 1;
                }
            }
            
            if(++offset > (MAX-1)) {
                offset = 0;
            }
        }
            break;
        case 1: {
            static int dir = 1;
            static int offset = 0;
            static int div = 2;
            static int size_y = rand()%(frame.rows-1);
            
            for(int z = 0; z < frame.rows; ++z) {
                for(int i = 0; i < frame.cols; ++i) {
                    cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                    int cy = AC_GetFZ(frame.rows-1, z, size_y);
                    if(cy >= 0 && cy < frame.rows && i >= 0 && i < frame.cols) {
                        cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(cy, i);
                        pixel = pix;
                    }
                }
                static int cnt = 0;
                if(++cnt > frame.rows/2) {
                    cnt = 0;
                    size_y = rand()%(frame.rows-1);
                }
            }
            
            if(++offset > (MAX-1)) {
                offset = 0;
            }
        }
            break;
        case 2: {
            static int dir = 1;
            static int offset = 0;
            static int size_y = frame.rows/16;
            static int size_x = 2;
            static int dir2 = 1;
            
            for(int z = 0; z < frame.rows; ++z) {
                for(int i = 0; i < frame.cols; ++i) {
                    cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                    int cy = AC_GetFZ(frame.rows-1, i, size_y);
                    
                    int cx = i/size_x;
                    
                    
                    if(cy >= 0 && cy < frame.rows && cx >= 0 && cx < frame.cols) {
                        cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(cy, cx);
                        pixel = pix;
                    }
                }
                
                if(dir == 1) {
                    size_y ++;
                    if(size_y > frame.rows*2)
                        dir = 0;
                } else {
                    size_y --;
                    if(size_y <= 2)
                        dir = 1;
                }
            }
            
            if(dir2 == 1) {
                size_x ++;
                if(size_x > 32)
                    dir2 = 0;
            } else {
                size_x --;
                if(size_x <= 2)
                    dir2 = 1;
            }
            
            if(++offset > (MAX-1)) {
                offset = 0;
            }
            
        }
            break;
    }
    
    
    if(++flash > 2)
        flash = 0;
    
}
