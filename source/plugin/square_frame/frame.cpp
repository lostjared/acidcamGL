#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    
    static int index = 0;
    static int col[3] = {0,1,2};
    static double level = 0.5;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b pix = ac::pixelAt(collection.frames[index], z, i);
            ++index;
            if(index > MAX-1)
                index = 0;
            
            pixel[0] = ac::wrap_cast((level * pixel[0] + (level * pix[col[0]])));
            pixel[1] = ac::wrap_cast((level * pixel[1] + (level * pix[col[1]])));
            pixel[2] = ac::wrap_cast((level * pixel[2] + (level * pix[col[2]])));
            
            for(int j = 0; j < 3; ++j) {
                col[j] ++;
                if(col[j] > 2) col[j] = 0;
            }
            
            level += 0.01;
            if(level > 0.5)
                level = 0.3;
        }
        ++index;
        if(index > MAX-1)
            index = 0;
        for(int j = 0; j < 3; ++j) {
            col[j] ++;
            if(col[j] > 2) col[j] = 0;
        }
        level += 0.01;
        if(level > 0.5)
            level = 0.3;
    }
}

