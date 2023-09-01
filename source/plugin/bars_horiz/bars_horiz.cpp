#include"ac.h"

void Glitch_Mirror_Bars_Horiz_drawMatrixRect(cv::Mat &frame, const cv::Mat &src, int row, int height) {
    int start_y = row;
    for(int y = start_y; y < start_y+height && y < frame.rows; ++y) {
        for(int x = 0; x < frame.cols; ++x) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
            const cv::Vec3b &pix = src.at<cv::Vec3b>(y, x);
            for(int q = 0; q < 3; ++q)
                pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));

        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
    collection.shiftFrames(frame);
    
    static int height_ = 25;
    static int num_rows = frame.rows/height_;
    
     for(int i = 1; i < num_rows; ++i) {
           Glitch_Mirror_Bars_Horiz_drawMatrixRect(frame, collection.frames[rand()%MAX], i*height_, height_);
     }
    
    static int dir  = 1;

       if(dir == 1) {
           height_ += 5;
           if(height_ >= frame.rows/4) {
               dir = 0;
           }
       } else {
           height_ -= 5;
           if(height_ <= 5) {
               dir = 1;
               height_ = 5;
           }
       }
}
