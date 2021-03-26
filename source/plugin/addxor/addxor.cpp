#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 4;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    int color[3] = {0,0,0};
    int off = 0;
     for(int z = 0; z < frame.rows; ++z) {
         for(int i = 0; i < frame.cols; ++i) {
             
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
             
             cv::Mat &m = collection.frames[off];
             cv::Vec3b pix = m.at<cv::Vec3b>(z, i);
             
             color[0] ^= pix[0];
             color[1] ^= pix[1];
             color[2] ^= pix[2];
             
             ++off;
             if(off > 7) {
                 off = 0;
                 color[0] = pix[0];
                 color[1] = pix[1];
                 color[2] = pix[2];
             }
            
             for(int q = 0; q < 3; ++q)
                 pixel[q] = static_cast<unsigned char>((0.5 * pixel[q]) + (0.5 * color[q]));
        }
    }
}
