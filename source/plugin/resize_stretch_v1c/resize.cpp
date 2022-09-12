#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(rand()%5 == 0) {
        collection.shiftFrames(frame);
    }
    
    static int wait = 0;
    static int timeout = 10+rand()%24;
    static int offset = 0;
    
    auto drawStretchedRect = [&](int y, int height, int nw, int nh) {
            
        for(int z = y; z < y+height; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                
                int ni = AC_GetFX(frame.cols-1, i, nw);
                int nz = AC_GetFZ(frame.rows-1, z, nh);
                
                if(ni > 0 && nz > 0 && ni < frame.cols && nz < frame.rows) {
                
                    pixel = collection.frames[offset].at<cv::Vec3b>(nz, ni);
                    
                }
            }
        }
        
    };
    
    int nw = frame.cols+(rand()%(frame.cols/4));
    int nh = frame.rows+(rand()%(frame.rows/4));
    
    auto drawRects = [&](int y, int &wait, int &timeout) {
   
        if(++wait > timeout) {
            wait = 0;
            timeout = 10+rand()%24;
            drawStretchedRect(y, frame.rows/4, nw, nh);
        }
        
    };
    int rnd = 1+(rand()%2);
    drawRects(0, wait, timeout);
    static int wt = 0, time2 = 0;
    drawRects((frame.rows/4)*rnd, wt, time2);
    
}
