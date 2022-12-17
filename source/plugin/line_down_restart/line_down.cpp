#include"ac.h"
#include<memory>


struct Impl {
    int *rows = nullptr;
    int *row_speed = nullptr;
    
    Impl() = default;
    ~Impl() {
        delete [] rows;
        delete [] row_speed;
    }
};



extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<2> collection;
    collection.shiftFrames(frame);
    static int row_w = 0;
    static int counter = 0;
    
    static Impl row;
    
    if(row_w == 0 || row_w != frame.cols) {
        
        if(row.rows != nullptr) {
            delete [] row.rows;
            delete [] row.row_speed;
        }
        
        row.rows = new int[frame.cols+1];
        row.row_speed =new int[frame.cols+1];
        for(int i = 0; i < frame.cols; ++i) {
            row.rows[i] = rand()%50;
            row.row_speed[i] = 2+rand()%25;
        }
        row_w = frame.cols;
        counter = 0;
    }
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            
            if(row.rows[i] == -1)
                continue;
            
            int row_y = row.rows[i]+z;
            if(row_y >= 0 && row_y < frame.rows) {
                cv::Vec3b &pix = collection.frames[1].at<cv::Vec3b>(row_y, i);
                for(int q = 0; q < 3; ++q)
                    pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
            }
        }
    }
    
    for(int i = 0; i < frame.cols; ++i) {
        row.rows[i] += row.row_speed[i];
        if(row.rows[i] != -1 && row.rows[i] > frame.rows) {
            row.rows[i] = -1;
            ++counter;
        }
    }
    
    if(counter >= row_w) {
        for(int i = 0; i < row_w; ++i) {
            row.rows[i] = 0;
        }
        counter = 0;
    }
}
