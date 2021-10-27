#include"ac.h"
#include<cstdlib>
#include<ctime>

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<8> collection;
    if(collection.empty())
        collection.shiftFrames(frame);
    int col_size = collection.size()-1;
    collection.shiftFrames(frame);
    static double alpha = 1.0;
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                double val[3] = {0};
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
                cv::Mat &m = collection.frames[7];
                cv::Vec3b one = m.at<cv::Vec3b>(z, i);
                cv::Mat &m1 = collection.frames[5];
                cv::Vec3b two = m1.at<cv::Vec3b>(z, i);
                cv::Mat &m2 = collection.frames[3];
                cv::Vec3b three = m2.at<cv::Vec3b>(z, i);
                val[0] = (one[0]+two[0]+three[0])*alpha;
                val[1] = (one[1]+two[1]+three[1])*alpha;
                val[2] = (one[2]+two[2]+three[2])*alpha;
                            
                for(int j = 0; j < 3; ++j)
                        pixel[j] = static_cast<unsigned char>(0.5 * pixel[j]) ^ static_cast<unsigned char>(0.5 * val[j]);
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);

    static int dir = 1;
    if(dir == 1) {
        alpha += 0.08;
        if(alpha >= 4)
            dir = 0;
    } else {
        alpha -= 0.08;
        if(alpha <= 1)
            dir = 1;
    }
}
