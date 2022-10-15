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
    
    auto drawRect = [&](cv::Mat &col_frame, int x, int y, int w, int h) {
      
        for(int z = y; z < y+h && z < frame.rows; ++z) {
            for(int i = x; i < x+w && i < frame.cols; ++i) {
                frame.at<cv::Vec3b>(z, i) = col_frame.at<cv::Vec3b>(z, i);
            }
        }
    };
    
    int size = frame.rows/4;
    static int offset = 0;
    
    for(int j = 0; j < 4; ++j) {
  
        if(!(rand()%4 == 0)) continue;
        
        drawRect(collection.frames[offset], 0, size*j, frame.cols, size);
    
        if(++offset > (MAX-1))
            offset = 0;
    }
    
}
