#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
        collection.shiftFrames(frame);
    
    static bool strobe = false;
    static int offset = 0;
    static int offset_r = rand()%(frame.cols-1), offset_g = rand()%(frame.cols-1), offset_b = rand()%(frame.cols-1);
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b pix[4];
            pix[0] = ac::pixelAt(collection.frames[1], z, offset_r);
            pix[1] = ac::pixelAt(collection.frames[4], z, offset_g);
            pix[2] = ac::pixelAt(collection.frames[7], z, offset_b);
            for(int j = 0; j < 3; ++j) {
                if(strobe == false)
                    pixel[j] = pix[offset][j];
                else
                    pixel[j] = pix[j][j];
            }
            ++offset_r;
            if(offset_r > frame.cols-1)
                offset_r = 0;
            ++offset_g;
            if(offset_g > frame.cols-1)
                offset_g = 0;
            ++offset_b;
            if(offset_b > frame.cols-1)
                offset_b = 0;
        }
        ++offset_r;
        if(offset_r > frame.cols-1)
            offset_r = 0;
        ++offset_g;
        if(offset_g > frame.cols-1)
            offset_g = 0;
        ++offset_b;
        if(offset_b > frame.cols-1)
            offset_b = 0;

    }
    strobe = (strobe == true) ? false : true;
    static int cnt = 0;
    static int dir = 1;
    if(++cnt > 6) {
        if(dir == 1) {
        ++offset;
            if(offset > 2) {
                dir = 0;
                offset = 2;
            }
            cnt = 0;
        } else {
            --offset;
            if(offset < 0) {
                offset = 0;
                dir = 1;
            }
        }
    }
}
