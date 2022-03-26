#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 32;
    static ac::MatrixCollection<MAX> collection;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(rand()%5 == 0)
        collection.shiftFrames(frame);
    
    static bool on = false;
    static int off = 0;
    static bool final_on = true;
    
    if(final_on == true) {
        
        for(int z = 0; z < frame.rows; ++z) {
            off = rand()%(frame.cols-1);
            for(int i = 0; i < frame.cols; ++i) {
                if(on == false) break;
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Mat &f = collection.frames[off];
                cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
                pixel = pix;
            }
            static int wait = rand()%50;
            static int cnt = 0;
            if(++cnt > wait) {
                cnt = 0;
                wait = rand()%50;
                on = !on;
                if(++off > MAX-1)
                    off = 0;
            }
        }
    } else {
        frame = collection.frames[off].clone();
    }
    
    static int final_wait = rand()%30;
    static int final_cnt = 0;
    
    if(++final_cnt > final_wait) {
        final_cnt = 0;
        final_wait = rand()%30;
        if(rand()%2 == 0)
            final_on = !final_on;
    }
    
}
