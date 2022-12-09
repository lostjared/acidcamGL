#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    ac::MatrixCollection<2> collection;
    collection.shiftFrames(frame);
    
    static int col_g = 0;
    
    static int offset_x = frame.cols;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = frame.cols; i > 0; --i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            int off_x = AC_GetFZ(frame.rows-1, i, offset_x);
            offset_x++;
            if(off_x >= 0 && off_x < frame.cols) {
                cv::Vec3b &pix = collection.frames[1].at<cv::Vec3b>(z, off_x);
                
                pixel[0] = pixel[0] & pix[0];
                pixel[1] = pixel[1] | pix[1];
                pixel[2] = pixel[2] ^ pix[2];
                
                pixel[0] = ac::wrap_cast(pixel[0]+col_g);
                pixel[2] = ac::wrap_cast(pixel[2]-col_g);
                pixel[1] = ac::wrap_cast(pix[1]+col_g);
                
            }
        }
        
        if(offset_x > (frame.cols*2)) {
            offset_x = frame.cols;
            col_g += 5;
            if(col_g > 255) col_g = 0;
        }
    }

}
