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
    
    int row = 0;
    static int offset = 0;
    static double index = 0.1;
    static int i_off = 0;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            
            cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            
            
            switch(i_off) {
                case 0: {
                    pixel[row] = ac::wrap_cast((index * pixel[row]) + (0.5-index * pix[row]));
                }
                    break;
                case 1: {
                    pixel[row] = ac::wrap_cast((0.5-index * pixel[row]) + (index * pix[row]));
                }
                    break;
                case 2: {
                    pixel[row] ^= (int) ac::wrap_cast((index * pixel[row]) + (0.5-index * pix[row]));
                }
                    break;
            }
         
            
            if(++row > 2) {
                row = 0;
                if(++i_off > 2)
                    i_off = 0;
            }
            
            index += 0.01;
        }
        
        if(index >= 0.5)
            index = 0.1;
        
        if(++offset > (MAX-1))
            offset = rand()%(MAX-1);
        
    }
}
