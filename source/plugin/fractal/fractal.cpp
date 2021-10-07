#include"ac.h"
#include<cstdlib>
#include<ctime>

namespace frac {
    double paramA = 1.0;
    double paramB = 1.0;
    int dir = 1;
    int MAX_ITER=80;
    void FractalLogic();
    void DrawFractal(cv::Mat &frame, bool neg=false);
    int mandelbrot(const std::complex<double> &c);
}

void frac::FractalLogic() {
    
    switch(dir) {
        case 1: {
            paramA += 0.005;
            if(paramA >= 1.0) {
                dir = 2;
                paramB += 0.05;
                if(paramB >= 1.0) {
                    dir = 3;
                    
                }
            }
        }
            break;
        case 2: {
            paramA -= 0.005;
            if(paramA <= -1) {
                paramB += 0.05;
                dir = 1;
                if(paramB >= 1.0) {
                    dir = 4;
                }
            }
        }
            break;
        case 3: {
            paramB -= 0.005;
            if(paramB <= -1.0) {
                dir = 3;
                paramA -= 0.05;
                if(paramA <= -1) {
                    dir = 1;
                }
            }
            
        }
            break;
        case 4: {
            paramB += 0.005;
            if(paramB >= 1) {
                paramA += 0.05;
                dir = 3;
                if(paramA >= 1) {
                    dir = 2;
                }
                
            }
        }
            
            break;
    }
}



int frac::mandelbrot(const std::complex<double> &c) {
    int n = 0;
    std::complex<double> z(0, 0);
    while(std::abs(z) <= 2 && n < MAX_ITER) {
        z = z*z+c;
        n ++;
    }
    return n;
}

void frac::DrawFractal(cv::Mat &frame, bool) {
    int width=frame.cols, height=frame.rows;
    double start = -2.0;
    double end = 2;
    double im_start = -1.0;
    double im_end = 1;
    static auto callback = [&](cv::Mat *frame, int offset, int cols, int size) {
        for(int z = offset; z <  offset+size; ++z) {
            for(int i = 0; i < cols; ++i) {
                cv::Vec3b &pixel = frame->at<cv::Vec3b>(z, i);
                double w = (double(i)/double(width));
                double h = (double(z)/double(height));
                std::complex<double> c(start + w * (end - start)*paramA, im_start + h * (im_end - im_start)*paramB);
                int n = mandelbrot(c);
                unsigned char color = static_cast<unsigned char>(255-(n * 255 / MAX_ITER));
                pixel[0] += color;
                pixel[1] += color;
                pixel[2] += color;
            }
        }
    };
    ac::UseMultipleThreads(frame, ac::getThreadCount(), callback);
}

extern "C" void filter(cv::Mat  &frame) {
    frac::DrawFractal(frame, false);
    //frac::FractalLogic();
}
