#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else if(rand()%3 == 0)
        collection.shiftFrames(frame);
    
    static bool strobe = false;
    static int offset = 0;
    static int index = 0;
    int offset_i = rand()%6;
    
    for(int z = 0; z < frame.rows; ++z) {
        static int off_x = rand()%(frame.cols-1);
        static int off_g = rand()%(frame.cols-1);
        static int off_b = rand()%(frame.cols-1);
        
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b pix[4];
            pix[0] = ac::pixelAt(collection.frames[1], z, off_x);
            pix[1] = ac::pixelAt(collection.frames[4], z, off_g);
            pix[2] = ac::pixelAt(collection.frames[7], z, off_b);
            
            pix[index] = ac::pixelAt(collection.frames[offset_i], z, i);
            
            for(int j = 0; j < 3; ++j) {
                if(strobe == false)
                    pixel[j] = pix[offset][j];
                else
                    pixel[j] = pix[j][j];
            }
            
            off_x += rand()%25;
            if(off_x > frame.cols-1)
                off_x = 0;
            off_g += rand()%25;
            if(off_g > frame.cols-1)
                off_g = 0;
            off_b += rand()%25;
            if(off_b > frame.cols-1)
                off_b = 0;
        }
    }
    
    ++index;
    if(index > 2)
        index = 0;
    
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
