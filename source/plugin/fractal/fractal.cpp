#include"ac.h"
#include<cstdlib>
#include<ctime>

namespace frac {
double paramA = -1.0;//0.519;
double paramB = 0.2;
float zoom_x = 1.5, zoom_y = 1.5, zoom_w = 0.5, zoom_h = 0.5;
double red_color = 6;
double green_color = 25;
double blue_color = 50;
float mod_x = 0.5;
float mod_y = 0.5;
float radius = 180.0f;
int dir = 1;
long max_iter = 60;
void FractalLogic();
void DrawFractal(cv::Mat &frame, bool neg=false);
}

void frac::FractalLogic() {
    static double alpha_r = 1.0;
    alpha_r += 0.05;
    red_color += alpha_r;
    green_color += alpha_r;
    blue_color += alpha_r;
    if(alpha_r > 255) alpha_r = 1.0;
    if(red_color > 255) red_color = rand()%255;
    if(green_color > 255) green_color = rand()%255;
    if(blue_color > 255) blue_color = rand()%255;
    
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
    // std::cout << paramA << ": " << paramB << " :" << " dir: " << dir << "\n";
}

int MAX_ITER=80;

int mandelbrot(const std::complex<double> &c) {
    int n = 0;
    std::complex<double> z(0, 0);
    while(std::abs(z) <= 2 && n < MAX_ITER) {
        z = z*z+c;
        n ++;
    }
    return n;
}

void frac::DrawFractal(cv::Mat &frame, bool)
{
    int width=frame.cols, height=frame.rows;
    double start = -2, end = 1, im_start = -1, im_end = 1;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double w = (double(x)/double(width));
            double h = (double(y)/double(height));
            std::complex<double> c(start + w * (end - start), im_start + h * (im_end - im_start));
            int n = mandelbrot(c);
            cv::Vec3b &pixel = frame.at<cv::Vec3b>(y, x);
            unsigned char color = static_cast<unsigned char>(255-(n * 255 / MAX_ITER));
            pixel[0] += color;
            pixel[1] += color;
            pixel[2] += color;
        }
    }
}

extern "C" void filter(cv::Mat  &frame) {
    frac::DrawFractal(frame, false);
    //frac::FractalLogic();
}
