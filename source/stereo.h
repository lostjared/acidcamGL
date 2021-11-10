#ifndef __STEREO__
#define __STEREO__


#ifdef _WIN32
#include"ac.h"
#else
#include"acidcam/ac.h"
#endif

namespace acidcam {
    void Stereo(cv::Mat &, cv::Mat &, cv::Mat &);
    class StereoCam {
    public:
       StereoCam();
       void Load(int *cmd);
       void Render(cv::Mat &frame);
        void SetSize(int w, int h, double fps_);
        cv::VideoCapture cap[2];
    private:
        int capture[2];
   };
}



#endif

