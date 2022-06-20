#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    
    collection.shiftFrames(frame);
    
    static int index = 0;
    static int offset = 0;
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
            pixel[0] = ac::wrap_cast((0.5 * pixel[0]) + (0.5 * pix[0]));
            pixel[1] = ac::wrap_cast((0.5 * pixel[1]) + (0.5 * pix[1]));
            pixel[2] = ac::wrap_cast((0.5 * pixel[2]) + (0.5 * pix[2]));
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
    
    static int f_index = 0;
    static int count = 0;
    
    switch(f_index) {
        case 0:
            ac::MirrorLeftBottomToTop(frame);
            break;
        case 1:
            ac::MirrorLeft(frame);
            break;
        case 2:
            ac::MirrorRight(frame);
            break;
        case 3:
            ac::MirrorLeftTopToBottom(frame);
            break;
        case 4:
            ac::MirrorFlipYMirrorLeft(frame);
            break;
        case 5:
            ac::MirrorFlipXMirrorLeft(frame);
            break;
        case 6:
            ac::MirrorRightTopToBottom(frame);
            break;
        case 7:
            ac::MirrorRightBottomToTop(frame);
            break;
        case 8:
            ac::MirrorFlipXMirrorRight(frame);
            break;
        case 9:
            ac::MirrorFlipYMirrorRight(frame);
            break;
    }
    
    static int dir1 = 1;
    
    if(dir1 == 1) {
        if(++count > ac::fps/2) {
            if(++f_index > 9) {
                f_index = 9;
                dir1 = 0;
            }
            count = 0;
        }
    } else {
        if(++count > ac::fps/2) {
            if(--index <= 0) {
                f_index = 0;
                dir1 = 1;
            }
        }
    }
    static int dir = 1;
    
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
}

