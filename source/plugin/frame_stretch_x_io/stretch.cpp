#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    
    static int off = 0;
    static int frame_x_off = 0;
    static int frame_y_off = 0;

    cv::Mat m = frame.clone();
    
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
        
                int off_x = AC_GetFX(frame->cols-1, i, frame->cols+frame_x_off);
                int off_y = AC_GetFZ(frame->rows-1, z, frame->rows+frame_y_off);
                
                cv::Vec3b pix = m.at<cv::Vec3b>(z, off_x);
                
                pixel[0] = ac::wrap_cast((0.5 * pixel[0]) + (0.5 * pix[0]));
                pixel[1] = ac::wrap_cast((0.5 * pixel[1]) + (0.5 * pix[1]));
                pixel[2] = ac::wrap_cast((0.5 * pixel[2]) + (0.5 * pix[2]));

            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
    
    static int dir = 1;
    static int frame_max = 1;
    if(dir == 1) {
        frame_x_off += 4;
        frame_y_off += 4;
        if((frame_x_off > frame_max) || (frame_y_off > frame_max))  {
            dir = 0;
            static int dir_max = 1;
            if(dir_max == 1) {
                frame_max += 50;
                if(frame_max >= frame.cols/2)
                    dir_max = 0;
            } else {
                frame_max -= 50;
                if(frame_max <= 0) {
                    frame_max = 0;
                    dir_max = 1;
                }
            }
            
        }
    } else {
        frame_x_off -= 4;
        frame_y_off -= 4;
        if(frame_x_off <= 0 || frame_y_off <= 0) {
            dir = 1;
            frame_x_off = 0;
            frame_y_off = 0;
        }
    }
}
