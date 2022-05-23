#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else if(rand()%2 == 0)
        collection.shiftFrames(frame);
    static bool strobe = false;
    static int offset = 0;
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b pix[4];
            pix[0] = ac::pixelAt(collection.frames[1], z, i);
            pix[1] = ac::pixelAt(collection.frames[4], z, i);
            pix[2] = ac::pixelAt(collection.frames[7], z, i);
            for(int j = 0; j < 3; ++j) {
                if(strobe == false)
                    pixel[j] = pix[offset][j];
                else
                    pixel[j] = pix[j][j];
            }
        }
    }
    strobe = (strobe == true) ? false : true;
    static int cnt = 0;
    if(++cnt > 4) {
        ++offset;
        if(offset > 2)
            offset = 0;
        cnt = 0;
    }
}
