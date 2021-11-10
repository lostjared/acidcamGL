#include"stereo.h"


void acidcam::Stereo(cv::Mat &frame, cv::Mat &img1, cv::Mat &img2) {
    cv::Mat frame1 = img1;
    cv::Mat frame2 = img2;
    cv::resize(frame1, img1, cv::Size(frame.cols/2, frame.rows));
    cv::resize(frame2, img2, cv::Size(frame.cols/2, frame.rows));

    for(int z = 0; z < frame.rows; ++z) {
        int x = 0;
        for(int i = frame.cols/2; i < frame.cols; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix = img2.at<cv::Vec3b>(z, x);
                ++x;
                pixel = pix;
            }
        for(int i = 0; i < frame.cols/2; ++i) {
                cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
                cv::Vec3b pix = img1.at<cv::Vec3b>(z, i);
                pixel = pix;
        }
    }
}



acidcam::StereoCam::StereoCam() {
    
}
void acidcam::StereoCam::Load(int *cmd) {
    capture[0] = cmd[0];
    capture[1] = cmd[1];
    for(int i = 0; i < 2; ++i) {
#ifndef _WIN32
        cap[i] = cv::VideoCapture(cmd[i]);
#else
        cap[i] = cv::VideoCapture(cmd[i], cv::CAP_DSHOW);
#endif
        if(!cap[i].isOpened()) {
            std::cout << "acidcam: couldn't open device: " << cmd[i] << "\n";
            exit(0);
        }
    }
}

void acidcam::StereoCam::Render(cv::Mat &frame) {
    cv::Mat img1, img2;
    if(cap[0].read(img1) && cap[1].read(img2)) {
       Stereo(frame, img1, img2);
    }
}

void acidcam::StereoCam::SetSize(int w, int h, double fps_) {
    std::cout << "acidcam: Stereo attempted resolution " << w << "x" << h << "p" << fps_ << "\n";
    for(int i = 0; i < 2; ++i) {
        cap[i].set(cv::CAP_PROP_FRAME_WIDTH, w);
        cap[i].set(cv::CAP_PROP_FRAME_HEIGHT, h);
        int zw = cap[i].get(cv::CAP_PROP_FRAME_WIDTH);
        int zh = cap[i].get(cv::CAP_PROP_FRAME_HEIGHT);
        cap[i].set(cv::CAP_PROP_FPS, fps_);
        double fps = cap[i].get(cv::CAP_PROP_FPS);
        std::cout << "acidcam: Stereo Mode: Camera: " << (i+1) << " Device: " << capture[i] << " @ " << zw << "x" << zh << "p" << fps << "\n";
    }
}

