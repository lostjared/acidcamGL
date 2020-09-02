
#ifndef  __FFMPEG_WRITE_H_
#define __FFMPEG_WRITE_H_

#include<iostream>
#include<string>
#include<cstdlib>
#include<opencv2/opencv.hpp>
#include<sstream>

FILE *open_ffmpeg(const char *output, const char *res, const char *dst_res, const char *fps, const char *crf);
void write_ffmpeg(FILE *fptr, cv::Mat &frame);
void mux_audio(const char *output, const char *src, const char *final_file);

/*
    FILE *fptr = open_ffmpeg("test.mp4", "640x480", "1440x1080", "30", "24");
    if(!fptr) {
        std::cout << "Error:\n";
        exit(0);
    }
    for(int i = 0; i < 120; ++i) {
        cv::Mat frame;
        acidcam::cap.read(frame);
        write_ffmpeg(fptr, frame);
    }
    pclose(fptr);
*/

#endif
