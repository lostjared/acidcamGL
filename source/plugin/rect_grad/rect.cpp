#include"ac.h"


void Glitch_Rect_Grad_drawBlock(double &alpha, int x, int y,  int w, int h, cv::Mat &frame, const cv::Mat &src) {
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
    static constexpr int num_rows = 8;
    static constexpr int num_cols = 8;
    static constexpr int arr_size = num_rows * num_cols;
    static double alpha_array[ arr_size ];
    for(int i = 0; i < arr_size; ++i) {
          alpha_array[i] = (0.1) * (rand()%10);
    }
    int row_size = frame.rows/num_rows;
    int col_size = frame.cols/num_cols;
    int index = 0;

      for(int y = 0; y < frame.rows; y += row_size) {
          for(int x = 0; x < frame.cols; x += col_size) {
              Glitch_Rect_Grad_drawBlock(alpha_array[index++], x, y, col_size, row_size, frame, collection.frames[rand()%(MAX-1)]);
          }
      }

      for(int i = 0; i < index; ++i) {
          alpha_array[i] += 0.05;
          if(alpha_array[i] >= 1.0)
              alpha_array[i] = 0.1;
      }
}
