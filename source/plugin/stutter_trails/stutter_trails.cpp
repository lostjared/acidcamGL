// This one runs really slow
#include"ac.h"
#include<cstdlib>
#include<ctime>

void stutter_filter(cv::Mat  &frame) {
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
            if((rand()%8)==0) {
                stored = frame.clone();
                on = !on;
            }
            frame = stored.clone();
        } else {
            if((rand()%5) == 0)
                on = !on;
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    cv::Mat out;
    stutter_filter(frame);
    ac::AlphaBlendDouble(frame, collection.frames[7],out,0.5, 0.5);
    collection.shiftFrames(out);
    frame = out.clone();
    ac::AddInvert(frame);
}
