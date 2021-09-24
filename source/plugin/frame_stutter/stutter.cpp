#include"ac.h"
#include<cstdlib>
#include<ctime>

extern "C" void filter(cv::Mat  &frame) {
    static cv::Mat stored;
    static cv::Size stored_size;
    
    if(stored_size != frame.size()) {
        srand(static_cast<int>(time(0)));
        stored = frame.clone();
        stored_size = frame.size();
    } else {
        if(stored.empty())
            stored = frame.clone();

        static bool on = true;
        if(on == true) {
            if((rand()%25)==0) {
                stored = frame.clone();
                on = !on;
            }
            frame = stored.clone();
        } else {
            if((rand()%30) == 0)
                on = !on;
        }
    }
}
