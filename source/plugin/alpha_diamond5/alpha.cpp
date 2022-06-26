#include"ac.h"

typedef void (*MirrorCall)(cv::Mat &frame);

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    else if(rand()%4 == 0)
        collection.shiftFrames(frame);
    
    static int index = 0;
    static int offset = 0;
    static double alpha = 0.1;
    
    int start_x = 0;
    int off_x = index;
    for(int z = 0; z < frame.rows; ++z) {
        start_x = off_x;
        for(int i = 0; i < frame.cols; ++i) {
            if(start_x > frame.cols-1) {
                start_x = 0;
            }
            cv::Vec3b &pixel = ac::pixelAt(frame,z, start_x);
            cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z, i);
            pixel[0] = ac::wrap_cast((alpha * pixel[0]) + ((1.0-alpha) * pix[0]));
            pixel[1] = ac::wrap_cast((alpha * pixel[1]) + ((1.0-alpha) * pix[1]));
            pixel[2] = ac::wrap_cast((alpha * pixel[2]) + ((1.0-alpha) * pix[2]));
            //            pixel = pix;
            ++start_x;
        }
        ++off_x;
        if(off_x > frame.cols)
            off_x = 0;
    }
    ++index;
    if(index > frame.cols)
        index = 0;
    
    static int f_index = rand()%9;
    static int count = 0;

    static MirrorCall m_call[] = { ac::MirrorLeftBottomToTop,
        ac::MirrorLeft,
        ac::MirrorRight,
        ac::MirrorLeftTopToBottom,
        ac::MirrorFlipYMirrorLeft,
        ac::MirrorFlipXMirrorLeft,
        ac::MirrorRightTopToBottom,
        ac::MirrorRightBottomToTop,
        ac::MirrorFlipXMirrorRight,
        ac::MirrorFlipYMirrorRight
    };
    
    m_call[f_index](frame);
    static int dir = 1;
    
    if(++f_index >= 9) {
        f_index = rand()%9;
    }
    
    if(dir == 1) {
        if(++offset > MAX-1) {
            offset = MAX-1;
            dir = 0;
        }
    } else {
        if(--offset <= 1) {
            offset = 1;
            dir = 1;
        }
    }
    
    static int alpha_dir = 1;
    
    if(alpha_dir == 1) {
        alpha += 0.05;
        if(alpha >= 1.0) {
            alpha = 1.0;
            alpha_dir = 0;
        }
    } else {
        alpha -= 0.05;
        if(alpha <= 0.1) {
            alpha = 0.1;
            alpha_dir = 1;
        }
    }
}

