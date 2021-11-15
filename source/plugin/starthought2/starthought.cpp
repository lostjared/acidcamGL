#include"ac.h"

unsigned char wrap_cast(double f) {
    unsigned long x = static_cast<unsigned long>(f);
    if(x > 255)
        x = x%255;
    return static_cast<unsigned char>(x);
}

extern "C" void filter(cv::Mat  &frame) {
    static double pos = 1.0;
    
    static int col[][2] = { {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}, {2, 2}};
    
    static int r = 1;
    static int e = 0;
    static int off = 2;

    
    auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &buffer = ac::pixelAt(*frame,z, i);
                if((i%off) == 0) {
                    if((z%off) == 0) {
                        buffer[col[r][0]] = wrap_cast(pos*buffer[e]-i);
                        buffer[col[r][1]] = wrap_cast((i-z)*pos);
                    } else {
                        buffer[col[r][0]] = wrap_cast(pos*buffer[e]-z);
                        buffer[col[r][1]] = wrap_cast((i+z)*pos);
                    }
                } else {
                    if((z%off) == 0) {
                        buffer[col[r][0]] = wrap_cast(1-pos*buffer[e]);
                        buffer[col[r][1]] = wrap_cast((i+z)*pos);
                    } else {
                        buffer[col[r][0]] = wrap_cast(pos*buffer[e]-z);
                        buffer[col[r][1]] = wrap_cast((i-z)*pos);
                    }
                }
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
    static double pos_max = 7.0f;
    static int direction = 1;
    ac::procPos(direction, pos, pos_max);

    ++e;
    if(e >= 2)
        e = 0;

    static int dir = 1;
    if(dir == 1) {
        ++off;
        if(off > 4)
            dir = 0;
    } else {
        --off;
        if(off <= 2)
            dir = 1;
    }
}

