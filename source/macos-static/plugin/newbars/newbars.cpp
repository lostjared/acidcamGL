// Goign to attempt to write something
// starting off slow
#include"ac.h"


extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    collection.shiftFrames(frame);
    
    int off = 0;
    int max = frame.rows / 8;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Vec3b pc = pixel;
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &pix = m.at<cv::Vec3b>(z, i);
            int rnd = rand()%3;
            
            switch(rand()%3) {
                    case 0:
                        pixel[0] = pixel[0] & pix[0];
                        pixel[1] = pixel[1] | pix[1];
                        pixel[2] = pixel[2] ^ pix[2];
                        break;
                    case 1:
                        pixel[0] = pixel[0] | pix[0];
                        pixel[1] = pixel[1] ^ pix[1];
                        pixel[2] = pixel[2] & pix[2];
                        break;
                    case 2:
                        pixel[0] = pixel[0] ^ pix[0];
                        pixel[1] = pixel[1] & pix[1];
                        pixel[2] = pixel[2] | pix[2];
                        break;
                }
            
            pixel[0] = static_cast<unsigned char> ((0.5 * pc[0]) + (0.5 * pixel[0]));
            pixel[1] = static_cast<unsigned char> ((0.5 * pc[1]) + (0.5 * pixel[1]));
            pixel[2] = static_cast<unsigned char> ((0.5 * pc[2]) + (0.5 * pixel[2]));
        }
        
        if((z%max) == 0)
            ++off;
        if(off > 7)
            off = 0;
    }

}
