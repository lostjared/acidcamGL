#include"ac.h"
#include<cstdlib>
#include<ctime>

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
    
    static int *across = 0;
    static int *across_dir = 0;
    static int across_size = 0;
    
    if(across_size == 0 || across_size != frame.rows) {
        if(across != 0) {
            delete [] across;
            delete [] across_dir;
        }
        across_size = frame.rows;
        across = new int[across_size+1];
        across_dir = new int[across_size+1];
        int across_height = 5+rand()%10;

        for(int q = 0; q < across_size; q += across_height) {
            int dir = rand()%2;
            int pos = rand()%(frame.cols-10);
            
            for(int e = 0; e < across_height && q+e < across_size; ++e) {
                across[q+e] = pos;
                across_dir[q+e] = dir;
            }
            across_height = 5+rand()%10;
        }
    }
    
    static int off = 0;
    
    for(int z = 0; z < frame.rows; ++z) {
        int offset = 0;
        
        for(int i = across[z]; i < frame.cols && offset < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[off];
            cv::Vec3b &col = m.at<cv::Vec3b>(z, offset);
            for(int q = 0; q < 3; ++q)
                pixel[q] = wrap_cast((0.5 * pixel[q]) + (0.5 * col[q]));
            offset++;
        }
        
        if(across_dir[z] == 0) {
            across[z] += 10+rand()%150;
            if(across[z] > frame.cols-5) {
                across_dir[z] = 1;
            }
        } else {
            across[z] -= 10+rand()%150;
            if(across[z] <= 1) {
                across_dir[z] = 0;
            }
        }
    }
    
    ++off;
    if(off > MAX-1) {
        off = 0;
    }
}
