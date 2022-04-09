#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 16;
    static ac::MatrixCollection<MAX> collection;
    static int max = 20;
    static int count = 0;
    
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    } else if(++count > max == 0) {
        collection.shiftFrames(frame);
        max = 1+(rand()%30);
        count = 0;
    }
    static int off = 0;
    cv::Mat &f = collection.frames[15];
    static bool on = true;
    
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
                cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
                if(on == true) {
                    pixel[0] = ac::wrap_cast ((0.5 * pixel[0]) + (0.5 * pix[0]));
                    
                    pixel[1] = ac::wrap_cast ((0.5 * pixel[1]) + (0.5 * pix[1]));
                    
                    pixel[2] = ac::wrap_cast ((0.5 * pixel[2]) + (0.5 * pix[2]));
                } else {
                    pixel = pix;
                }
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);

    
    static int wait = rand()%30;
    static int counter = 0;
    if(++counter > wait) {
        counter = 0;
        wait = rand()%30;
        on = !on;
    }
}
