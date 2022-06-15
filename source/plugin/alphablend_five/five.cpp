#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    collection.shiftFrames(frame);
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            
            cv::Vec3b pix[3];
            pix[0] = ac::pixelAt(collection.frames[0], z, i);
            pix[1] = ac::pixelAt(collection.frames[3], z, i);
            pix[2] = ac::pixelAt(collection.frames[7], z, i);
            
            pixel[0] = (0.33 * pix[0][0]) + (0.33 * pix[1][0]) + (0.33 * pix[2][0]);
            pixel[1] = (0.33 * pix[0][1]) + (0.33 * pix[1][1]) + (0.33 * pix[2][1]);
            pixel[2] = (0.33 * pix[0][2]) + (0.33 * pix[1][2]) + (0.33 * pix[2][2]);

        }
    }
}
