#include"ac.h"

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    else
    collection.shiftFrames(frame);
    double alpha = 1.0;
    double diff = 1.0/double(frame.rows);
      alpha = (rand()%10) * 0.1;
    static int index = 0;
    
    ++index;
    if(index > (MAX-1))
        index = 0;

    cv::Mat &frame2 = collection.frames[index];
    
      for(int z = 0; z < frame.rows; ++z) {
          for(int i = 0; i < frame.cols; ++i) {
              cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
              const cv::Vec3b &pix = frame2.at<cv::Vec3b>(z, i);
              for(int q = 0; q < 3; ++q) {
                  pixel[q] = ac::wrap_cast((alpha * pixel[q]) + ((1-alpha)*pix[q]));
              }
          }
          alpha += diff;
      }
}
