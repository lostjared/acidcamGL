#include"ac.h"
#include<cstdlib>
#include<ctime>
#include<memory>

unsigned char wrap_cast(double d) {
    unsigned long x = static_cast<unsigned long>(d);
    if(x > 255)
        x = x%255;
    return static_cast<unsigned char>(x);
}

extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    if(collection.empty()) {
        collection.shiftFrames(frame);
        srand(static_cast<unsigned int>(time(0)));
    }
    collection.shiftFrames(frame);
    
    static std::unique_ptr<int> across;
    static std::unique_ptr<int> across_dir;
    static int across_size = 0;
    
    if(across_size == 0 || across_size != frame.rows) {
        across_size = frame.rows;
        across.reset(new int[across_size+1]);
        across_dir.reset(new int[across_size+1]);
        int across_height = 5+rand()%10;

        for(int q = 0; q < across_size; q += across_height) {
            int dir = rand()%2;
            int pos = rand()%(frame.cols-10);
            
            for(int e = 0; e < across_height && q+e < across_size; ++e) {
                across.get()[q+e] = pos;
                across_dir.get()[q+e] = dir;
            }
            across_height = 5+rand()%10;
        }
    }
    
    static int off = 0;
    
    for(int z = 0; z < frame.rows; ++z) {
        int offset = 0;
        
        for(int i = across.get()[z]; i < frame.cols && offset < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &col = m.at<cv::Vec3b>(z, offset);
            for(int q = 0; q < 3; ++q)
                pixel[q] = wrap_cast((0.5 * pixel[q]) + (0.5 * col[q]));
            offset++;
        }
        
        if(across_dir.get()[z] == 0) {
            across.get()[z] += 10+rand()%150;
            if(across.get()[z] > frame.cols-5) {
                across_dir.get()[z] = 1;
            }
        } else {
            across.get()[z] -= 10+rand()%150;
            if(across.get()[z] <= 1) {
                across_dir.get()[z] = 0;
            }
        }
    }
    
    ++off;
    if(off > MAX-1) {
        off = 0;
    }
}
