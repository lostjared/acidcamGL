#include"ac.h"

void Bars_Col_Alpha_drawMatrix(double &alpha, cv::Mat &frame, const cv::Mat &src, int rev, int col, int sizex) {
    int width = sizex;
    int start_x = col*width;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = start_x; i < start_x+width && i < frame.cols; i++) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            if(rev == 0 && frame.cols-i-1 >= 0 && frame.cols-i-1 < frame.cols) {
                cv::Vec3b pix = src.at<cv::Vec3b>(z, frame.cols-i-1);
                for(int q = 0; q < 3; ++q)
                   pixel[q] = ac::wrap_cast((alpha * pixel[q]) + ((1-alpha) * pix[q]));
            } else {
                cv::Vec3b pix = src.at<cv::Vec3b>(z, i);
                for(int q = 0; q < 3; ++q)
                   pixel[q] = ac::wrap_cast((alpha * pixel[q]) + ((1-alpha) * pix[q]));
            }
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static int num_rows = 64;
    static int dir = 1;
    static double alpha = 1.0;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
    collection.shiftFrames(frame);

    for(int i = 0; i < frame.cols/num_rows; ++i) {
          cv::Mat &fx = collection.frames[rand()%collection.size()];
          Bars_Col_Alpha_drawMatrix(alpha, frame, fx, 1, i,  num_rows);
      }
      if(dir == 1) {
          num_rows += 1;
          if(num_rows >= 128)
              dir = 0;
      } else {
          num_rows -= 1;
          if(num_rows <= 64)
              dir = 1;
      }
    
    static int alpha_dir = 1;
    if(alpha_dir == 1) {
        alpha += 0.05;
        if(alpha >= 1.0) {
            alpha_dir = 0;
        }
    } else {
        alpha -= 0.05;
        if(alpha <= 0.1) {
            alpha_dir = 1;
        }
    }
}
