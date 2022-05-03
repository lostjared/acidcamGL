#include"ac.h"
#include<list>

struct Pixel {
    int x,y;
    cv::Vec3b color;
    int timeout;
    Pixel() : x{0}, y{0}, timeout{0} {}
    Pixel(int xx, int yy, cv::Vec3b c) : x{xx}, y{yy}, color{c}, timeout{0} {}
    
};


extern "C" void filter(cv::Mat  &frame) {
    
    static constexpr int MAX = 8;
    static ac::MatrixCollection<MAX> collection;
    static std::list<Pixel> pixels;
    static int counter = 0;

    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    } else {
        collection.shiftFrames(frame);
    }
    
    for(int z = 0; z < frame.rows-20; z += 20) {
        for(int i = 0; i < frame.cols-20; i += 20) {
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
            cv::Mat &m = collection.frames[2];
            cv::Vec3b pix = m.at<cv::Vec3b>(z, i);
            bool set = false;
            for(int q = 0; q < 3; ++q) {
                if(abs(pixel[q]-pix[q]) > 40) {
                    set = true;
                    break;
                }
            }
            if(set == true) {
                pixels.push_back(Pixel(i, z, cv::Vec3b(rand()%255, rand()%255, rand()%255)));
            }
        }
    }
    
    for(std::list<Pixel>::iterator i = pixels.begin(); i != pixels.end(); ) {
        if(++i->timeout > 3) {
            i = pixels.erase(i);
        } else {
                        
        for(int y = i->y; y < i->y + 25; ++y)
            for(int x = i->x; x < i->x + 25; ++x) {
                if(x >= 0 && x < frame.cols && y >= 0 && y < frame.rows) {
                    i->color = cv::Vec3b(rand()%255, rand()%255, rand()%255);
                    cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
                    pixel = i->color;
                    //pixel = pix;
                }
            }
            
            i ++;
        }
    }
}
