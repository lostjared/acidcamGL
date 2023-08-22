#include"ac.h"

void Bars_Col_drawMatrix(cv::Mat &frame, const cv::Mat &src, int rev, int col, int sizex) {
    int width = sizex;
    int start_x = col*width;
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = start_x; i < start_x+width && i < frame.cols; i++) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            if(rev == 0 && frame.cols-i-1 >= 0 && frame.cols-i-1 < frame.cols) {
                cv::Vec3b pix = src.at<cv::Vec3b>(z, frame.cols-i-1);
                for(int q = 0; q < 3; ++q)
                   pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
            } else {
                cv::Vec3b pix = src.at<cv::Vec3b>(z, i);
                for(int q = 0; q < 3; ++q)
                   pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
            }
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static int num_rows = 64;
    static int dir = 1;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
    collection.shiftFrames(frame);

    for(int i = 0; i < frame.cols/num_rows; ++i) {
          cv::Mat &fx = collection.frames[rand()%collection.size()];
          Bars_Col_drawMatrix(frame, fx, 1, i,  num_rows);
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
}
