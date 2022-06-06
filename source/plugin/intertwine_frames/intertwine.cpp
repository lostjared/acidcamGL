#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX=240;
    cv::Mat reframe;
    cv::resize(frame, reframe, cv::Size(320, MAX));
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(reframe);
    }
    else
        collection.shiftFrames(reframe);
    
    static bool strobe = false;
    static int offset = 0;
    for(int z = 0; z < reframe.rows; ++z) {
        for(int i = 0; i < reframe.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(reframe, z, i);
            pixel = collection.frames[z].at<cv::Vec3b>(z, i);
            
        }
    }
    cv::resize(reframe, frame, frame.size());
}
