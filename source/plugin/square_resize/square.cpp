#include"ac.h"
#include<cstdlib>
#include<ctime>

static constexpr int MAX = 8;


unsigned char wrapToChar(double d) {
    if(d < 255) {
        unsigned long x = static_cast<unsigned long>(d);
        return static_cast<unsigned char>(x);
    }
    unsigned long x = static_cast<unsigned long>(d);
    return static_cast<unsigned char>(x%255);
}

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
            if((rand()%3)==0) {
                stored = frame.clone();
                on = !on;
            }
            frame = stored.clone();
        } else {
            if((rand()%4) == 0)
                on = !on;
        }
    }
}

class Square {
public:
    int size_x, size_y;
    int offset;
    Square() : size_x{0},size_y{0}, offset{0} {}
    Square(int sx, int sy, int o) : size_x{sx},size_y{sy}, offset{o} {}
};

class Squares {
    cv::Size size_val;
public:
    Squares() : square{nullptr} {
    }
    
    ~Squares() {
        release();
    }
    
    void init(cv::Size s) {
        size_val = s;
        if(square != nullptr)
            release();
        
        square = new Square*[s.width];
        for(int i = 0; i < s.width; ++i) {
            square[i] = new Square[s.height];
            for(int z = 0; z < s.height; ++z) {
                square[i][z] = Square(4+rand()%28, 4+rand()%28, rand()%(MAX-1));
            }
        }
    }
    
    Square &operator()(int x, int y) {
        return square[x][y];
    }
    
    void release() {
        if(square != nullptr) {
            for(int i = 0; i < size_val.width; ++i)
                delete square[i];
            delete [] square;
            square = nullptr;
        }
    }
protected:
    Square **square;
};

extern "C" void filter(cv::Mat  &frame) {
    static ac::MatrixCollection<MAX> collection;
    static Squares squarez;
    static cv::Size size_val;
    
    if(collection.empty()) {
        srand(static_cast<unsigned int>(time(0)));
        collection.shiftFrames(frame);
    }
    if(size_val != frame.size()) {
        squarez.init(frame.size());
        size_val = frame.size();
    }
    stutter_filter(frame);
    collection.shiftFrames(frame);
    int square_size_x = squarez(0, 0).size_x;
    int square_size_y = squarez(0, 0).size_y;
    static int offset = squarez(0, 0).offset;
    for(int z = 0; z < frame.rows-square_size_y; z += square_size_y) {
        for(int i = 0; i < frame.cols-square_size_x; i += square_size_x) {
            for(int x = 0; x+i < frame.cols && x < square_size_x; ++x) {
                for(int y = 0; z+y < frame.rows && y < square_size_y; ++y) {
                    cv::Vec3b &pixel = ac::pixelAt(frame,z+y, i+x);
                    cv::Vec3b pix = collection.frames[offset].at<cv::Vec3b>(z+y, i+x);
                    for(int j = 0; j < 3; ++j)
                        pixel[j] = wrapToChar((0.5 * pixel[j]) + (0.5 * pix[j]));
                }
                square_size_x = squarez(i, z).size_x;
                square_size_y = squarez(i, z).size_y;
                offset = squarez(i, z).offset;
            }
        }
       
    }
}
