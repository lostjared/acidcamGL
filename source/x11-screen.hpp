// modified from stackoverflow
#ifndef __X11_SS_H_
#define __X11_SS_H_
#include<opencv2/opencv.hpp>
#include <X11/Xutil.h>
class ScreenShot
{
    Display* display;
    Window root;
    XImage* img{nullptr};
public:
    
    ScreenShot()
    {
        display = XOpenDisplay(nullptr);
        root = DefaultRootWindow(display);
    }

    void operator() (cv::Mat& cvImg, int x, int y, int width, int height)
    {
        if(img != nullptr)
            XDestroyImage(img);
        img = XGetImage(display, root, x, y, width, height, AllPlanes, ZPixmap);
        cvImg = cv::Mat(height, width, CV_8UC4, img->data);
    }

    ~ScreenShot()
    {
        if(img != nullptr)
            XDestroyImage(img);
        XCloseDisplay(display);
    }
};
#endif
