#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
  
    static int offset_x = 0;
    static int max_x = frame.cols+(frame.cols/2);
    
    ac::MatrixCollection<2> collection;
    collection.shiftFrames(frame);
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int x = 0; x < frame.cols; x += 100) {
            for(int i = 0; i < 100; ++i) {
                int pos_x = x+i;
                int px = AC_GetFZ(frame.cols, pos_x, offset_x);
                if(px >= 0 && px < frame.cols) {
                    cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, x+i);
                    cv::Vec3b &pix = collection.frames[1].at<cv::Vec3b>(z, px);
                    pixel = pix;
                }
            }
            offset_x --;
            if(offset_x <= max_x) {
                offset_x = frame.cols+(frame.cols/2);
                static int dir = 0;
                if(dir == 1) {
                    max_x += 10;
                    if(max_x > (frame.cols+(frame.cols/2)))
                        dir = 0;
                } else {
                    max_x -= 10;
                    if(max_x <= 100)
                        dir = 1;
                }
            }
        }
    }
    
}
