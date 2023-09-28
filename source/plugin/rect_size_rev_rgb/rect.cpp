#include"ac.h"


void Glitch_Rect_Size_drawBlock(int channel, double &alpha, int x, int y,  int w, int h, cv::Mat &frame, const cv::Mat &src) {
    for(int z = y; z < y+h && z < frame.rows; ++z) {
        for(int i = x;  i < x+w && i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            const cv::Vec3b &pix = src.at<cv::Vec3b>(z, i);
            pixel[channel] = ac::wrap_cast((alpha * pixel[channel]) + ((1-alpha)*pix[channel]));
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
    static int num_rows = 8;
    static int num_cols = 64;
    
    int row_size = frame.rows/num_rows;
    int col_size = frame.cols/num_cols;
    
    for(int y = 0; y < frame.rows; y += row_size) {
        for(int x = 0; x < frame.cols; x += col_size) {
            double alpha = 0.5;
            Glitch_Rect_Size_drawBlock(rand()%3, alpha, x, y, col_size, row_size, frame, collection.frames[rand()%(MAX-1)]);
        }
    }
    
    static int num_dir = 1;
    if(num_dir == 1) {
        num_rows += 2;
        num_cols -= 2;
        if(num_rows >= 64) {
            num_dir = 0;
        }
        if(num_cols <= 4) {
            num_dir = 0;
        }
    } else {
        num_rows -= 2;
        num_cols += 2;
        if(num_rows <= 4) {
            num_dir = 1;
        }
        if(num_cols >= 64) {
            num_dir = 1;
        }
    }
}
