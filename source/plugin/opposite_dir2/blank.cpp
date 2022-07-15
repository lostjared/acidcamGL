#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    else if(rand()%3 == 0)
        collection.shiftFrames(frame);
    
    static int offset = 0;
    static int cx = rand()%50;
    static int max_cx = rand()%100;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q)
                pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));

        }
        static int c = 0;
        if(++c > cx) {
            if(++offset >(MAX-1))
                offset = 0;
            c = 0;
            if(++cx > max_cx) {
                cx = rand()%50;
                max_cx = rand()%100;
            }
        }

    }
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = ac::pixelAt(frame, z, i);
            cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q)
                pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
        }
        static int c = 0;
        if(++c > cx) {
            if(--offset <= 0)
                offset = MAX-1;
            c = 0;
            if(++cx > max_cx) {
                cx = rand()%50;
                max_cx = rand()%100;
            }
        }
    }
}
