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
    
    static int wait = rand()%30;
    static int counter = 0;
    
    if(++counter > wait) {
        counter = 0;
        wait = rand()%30;
        
        int row = 0;
        static int offset = 0;
        
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, i);
                
                
                pixel[row] ^= pix[row];
            }
            if(++row > 2)
                row = 0;
            if(++offset > (MAX-1))
                offset = rand()%(MAX-1);
            
        }
    }
}
