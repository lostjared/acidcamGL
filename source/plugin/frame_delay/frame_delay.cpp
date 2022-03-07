#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(rand()%2==0)
        collection.shiftFrames(frame);
    static bool on = false;
    static int off = 0;
    static int line_start_y = 2+rand()%(frame.rows/2);
    static int line_stop_y = line_start_y+(rand()%(frame.rows/2));
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            if(z >= line_start_y && z < line_stop_y) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Mat &f = collection.frames[7];
                cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
                pixel = pix;
            }
        }
    }
    static int wait = 30+rand()%100;
    static int duration = 0;
    ++duration;
    if(duration > wait) {
        duration = 0;
        wait = 30+rand()%100;
        line_start_y = 2+rand()%(frame.rows/2);
        line_stop_y = line_start_y+(rand()%(frame.rows/2));
    }
}
