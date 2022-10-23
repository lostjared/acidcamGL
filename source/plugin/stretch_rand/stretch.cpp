#include"ac.h"
#include<list>

struct AC_Rect {
    int x,y,w,h;
    int offset;
    bool dir;
    int ex_w, ex_h;
    AC_Rect() : x{0}, y{0}, w{0}, h{0}, offset{0} {}
    void setRect(int xx, int yy, int ww, int hh) {
        x = xx;
        y = yy;
        w = ww;
        h = hh;
        ex_w = ww+50;
        ex_h = hh+50;
        dir = (rand()%2 == 0) ? true : false;
    }
};

class RectContainer {
    int size_w, size_h;
public:
    std::list<AC_Rect> cont;
    RectContainer() = default;
    void init(int MAX, int max, int w, int h) {
        size_w = w;
        size_h = h;
        for(int i = 0; i < max; ++i) {
            AC_Rect rc;
            rc.setRect(rand()%w, rand()%h, rand()%w, rand()%h);
            rc.offset = rand()%MAX;
            cont.push_back(rc);
        }
    }
    void update(int MAX, int w, int h, int step) {
        for(auto it = cont.begin(); it != cont.end(); ++it) {
            it->x += rand()%(1+step);
            it->y += rand()%(1+step);
            it->w += rand()%(1+step);
            it->h += rand()%(1+step);
            
            if(it->dir == true) {
                if(it->offset ++ > (MAX-1)) {
                    it->offset = MAX-1;
                    it->dir = false;
                }
            } else {
                if(it->offset-- <= 0) {
                    it->offset = 0;
                    it->dir = true;
                }
            }
            if(it->w > 50 || it->h > h) {
                it->x = rand()%size_w;
                it->y = rand()%size_h;
                it->w = rand()%size_w;
                it->h = rand()%size_h;
                continue;
            }
        }
    }
    
};


extern "C" void filter(cv::Mat  &frame) {
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    static RectContainer cont;
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        cont.init(MAX, 20, frame.cols, frame.rows);
        collection.shiftFrames(frame);
    }
    else
        collection.shiftFrames(frame);
    
    cont.update(MAX, frame.cols, frame.rows, rand()%10);
    for(auto i = cont.cont.begin(); i != cont.cont.end(); ++i) {
        for(int x = i->x; x < i->x + i->w && x < frame.cols; ++x) {
            for(int y = i->y; y < i->y + i->h && y < frame.rows; ++y) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
                cv::Vec3b &pix = collection.frames[i->offset].at<cv::Vec3b>(y, x);
                for(int q = 0; q < 3; ++q) {
                    pixel[q] = ac::wrap_cast((0.5 * pixel[q]) + (0.5 * pix[q]));
                }
            }
        }
    }
}
