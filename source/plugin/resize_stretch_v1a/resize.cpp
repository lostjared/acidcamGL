#include"ac.h"




extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    static int offset = 0;
    auto stretch = [](cv::Mat &frame, ac::MatrixCollection<MAX> &col, int offset, int x, int new_h) {
        for(int y = 0; y < frame.rows; ++y) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
            int nh = AC_GetFZ(frame.rows-1, y, new_h);
            if(nh > 0 && nh < frame.rows) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(nh, x);
                pixel = pix;
            }
        }
    };
    
    static int wait = 0;
    static int timeout = 10+rand()%30;
    if(++wait > timeout) {
        
        wait = 0;
        timeout = 10+rand()%30;
        int new_h = rand()%(frame.rows-1);
        
        for(int i = 0; i < frame.cols; ++i) {
            
            if( (i%(frame.cols/4) == 0)) {
                new_h = rand()%(frame.rows-1);
                offset ++;
                if(offset > (MAX-1))
                    offset = 0;
            }
            
            stretch(frame, collection, offset, i, new_h);
            
        }
        
        ++offset;
        if(offset > (MAX-1))
            offset = 0;
    }
}
