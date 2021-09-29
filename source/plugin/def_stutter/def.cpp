// This one runs really slow
#include"ac.h"
#include<cstdlib>
#include<ctime>

void stutter_filter(cv::Mat  &frame) {
    static cv::Mat stored;
    static cv::Size stored_size;
    
    if(stored_size != frame.size()) {
        srand(static_cast<int>(time(0)));
        stored = frame.clone();
        stored_size = frame.size();
    } else {
        if(stored.empty())
            stored = frame.clone();

        static bool on = true;
        if(on == true) {
            if((rand()%25)==0) {
                stored = frame.clone();
                on = !on;
            }
            frame = stored.clone();
        } else {
            if((rand()%30) == 0)
                on = !on;
        }
    }
}




extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty())
        srand(static_cast<unsigned int>(time(0)));
    collection.shiftFrames(frame);
    cv::Mat frame_copy = frame.clone();
    stutter_filter(frame_copy);
    int num_pixels = rand()%((frame.rows * frame.cols)/320);
    for(int q = 0; q < num_pixels; ++q) {
       int pixel_size = 16+(rand()%64);
       
       int off_x = rand()%(frame.cols-pixel_size-1);
       int off_y = rand()%(frame.rows-pixel_size-1);
       int offset = rand()%(collection.size()-1);
       
       for(int y = off_y; y < off_y + pixel_size; ++y) {
           for(int x = off_x; x < off_x + pixel_size; ++x) {
               cv::Mat &m = collection.frames[offset];
               cv::Vec3b &pix = m.at<cv::Vec3b>(y, x);
               cv::Vec3b &pix_copy = frame_copy.at<cv::Vec3b>(y, x);
               cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
               for(int j = 0; j < 3; ++j) {
                   if(pixel[j] != pix[j]) {
                        pixel[j] = static_cast<unsigned char>((0.5 * pixel[j]) + (0.5 * pix_copy[j]));
                    }
               }
           }
       }
   }
}
