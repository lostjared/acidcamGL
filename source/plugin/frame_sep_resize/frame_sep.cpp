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
    
    cv::Mat &old = collection.frames[7];
    ac::Square_Block_Resize_Vertical(old);
 
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            if(on == false) break;
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &f = collection.frames[off];
            cv::Vec3b pix = f.at<cv::Vec3b>(z, i);
             cv::Vec3b &old_pix = old.at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q) {
                if(abs(pixel[q]-old_pix[q]) > 25) {
                    pixel[q] = pix[q];
                }
            }
        }
        if(((z%240)==0) && ++off > MAX-1) {
            off = 0;
            static int cnt = 0;
            static int wait = rand()%10;
            ++cnt;
            if(cnt > wait) {
                on = !on;
                cnt = 0;
                wait = rand()%10;
            }
        }
    }
}
