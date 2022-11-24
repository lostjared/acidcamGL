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
            for(int z = 0; z < frame.rows; ++z) {
                for(int i = 0; i < frame.cols; ++i) {
                    cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
                    cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, i);
                    for(int q = 0; q < 3; ++q) {
                        pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
                    }
                }
            }
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
        case 1: {
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
            break;
        case 2: {
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
    }
    
    if(++flash > 2)
        flash = 0;
    
}
