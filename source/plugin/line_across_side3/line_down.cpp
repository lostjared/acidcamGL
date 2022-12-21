#include"ac.h"

struct LineAcrossImpl {
    struct Item {
        int col, speed;
        bool on;
        Item() : col{rand()%50}, speed{rand()%25}, on{true}  {}
    };
    Item *item = nullptr;
    LineAcrossImpl() = default;
    ~LineAcrossImpl() {
        if(item != nullptr)
            delete [] item;
        
    }
};


extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    collection.shiftFrames(frame);
    static int row_w = 0;
    static int offset = 0;
    static LineAcrossImpl row;
    
    if(row_w == 0 || row_w != frame.rows) {
        if(row.item != nullptr) {
            delete [] row.item;
        }
        row_w = frame.rows;
        row.item = new LineAcrossImpl::Item[row_w+1];
    }
    
    for(int z = 0; z < frame.rows; ++z) {
        for(int i = 0; i < frame.cols; ++i) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            
            int row_col = row.item[z].col+i;
            int row_x = AC_GetFX(frame.cols-1, i, row_col);
            
            if(row_x >= 0 && row_x < frame.cols) {
                cv::Vec3b &pix = collection.frames[offset].at<cv::Vec3b>(z, row_x);

                for(int q = 0; q < 3; ++q)
                    pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
            }
        }
        
        if(++offset > (MAX-1)) {
            offset = rand()%(MAX-1);
        }
    }
    
    for(int i = 0; i < row_w; ++i) {
        if(row.item[i].on) {
            row.item[i].col += 25;
            if(row.item[i].col > frame.cols)
                row.item[i].on = false;
        } else {
            row.item[i].col -= 25;
            if(row.item[i].col <= 1) {
                row.item[i].col = 1;
                row.item[i].on = true;
            }
        }
    }
    
   
}
