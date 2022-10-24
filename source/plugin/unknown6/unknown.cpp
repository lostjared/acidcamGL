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
    
    static int dir = 1;
    static int offset = 0;
    static int nw = frame.cols;
    static int nh = frame.rows;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            int offx = AC_GetFX(frame.cols-1, i, nw);
            int offy = AC_GetFZ(frame.rows-1, z, nh);
            cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(offy, offx);
            
            for(int q = 0; q < 3; ++q) {
                pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
            }
            
        }
    }
 
    if(dir == 1) {
        if(++offset > (MAX-1)) {
            offset = MAX-1;
            dir = 0;
        }
        
        nw += (rand()%5);
        nh += (rand()%5);
        
        if(nw > frame.cols+(frame.cols/32)) {
            nw = frame.cols;
        }
        
        if(nh > frame.rows+(frame.rows/32)) {
            nh = frame.rows;
        }
        
    } else {
        if(--offset <= 0) {
            offset = 0;
            dir = 1;
        }
    }
}
