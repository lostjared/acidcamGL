#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(rand()%5==0)
        collection.shiftFrames(frame);
    static bool on = false;
    static int off = 0;
    if(on == true) {
        for(int z = 0; z < frame.rows; ++z) {
            for(int i = 0; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Mat &f = collection.frames[off];
                cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
                for(int q = 0; q < 3; ++q)
                    pixel[q] = ac::wrap_cast((pixel[q] * 0.5) + (pix[q] * 0.5));
            }
        }
    }
    
    
    static int dir = 1;
    
    if(dir == 1) {
        off++;
        if(off > MAX-1) {
            off = MAX-1;
            dir = 0;
        }
    } else {
        off--;
        if(off <= 0) {
            off = 0;
            dir = 1;
        }
    }
    static int cnt = 0;
    static int wait = rand()%20;
    ++cnt;
    if(cnt > wait) {
        on = !on;
        cnt = 0;
        wait = rand()%20;
    }
}
