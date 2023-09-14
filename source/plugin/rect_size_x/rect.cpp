#include"ac.h"


void Glitch_Rect_SizeX_drawBlock(double &alpha, int x, int y,  int w, int h, cv::Mat &frame, const cv::Mat &src) {
    for(int z = y; z < y+h && z < frame.rows; ++z) {
        for(int i = x;  i < x+w && i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            const cv::Vec3b &pix = src.at<cv::Vec3b>(z, i);
            for(int q = 0; q < 3; ++q)
                pixel[q] = ac::wrap_cast((alpha * pixel[q]) + ((1-alpha)*pix[q]));
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
    static int num_cols = 8;
    
    int row_size = frame.rows/num_rows;
    int col_size = frame.cols/num_cols;
    int index = 0;
    double alpha_inc = 1.0/double(num_cols);
    for(int y = 0; y < frame.rows; y += row_size) {
        double alpha_y = 1.0;
        for(int x = 0; x < frame.cols; x += col_size) {
            double alpha = alpha_y;
            Glitch_Rect_SizeX_drawBlock(alpha, x, y, col_size, row_size, frame, collection.frames[index]);
            
            index ++;
            if(index > static_cast<int>(MAX)-1)
                index = 0;
            
            alpha_y -= alpha_inc;
            
        }
    }
    
    static int num_dir = 1;
    if(num_dir == 1) {
        num_rows += 2;
        num_cols += 2;
        if(num_rows >= 64) {
            num_dir = 0;
        }
    } else {
        num_rows -= 2;
        num_cols -= 2;
        if(num_rows <= 4) {
            num_dir = 1;
        }
    }
}
