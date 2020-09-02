
#ifndef  __FFMPEG_WRITE_H_
#define __FFMPEG_WRITE_H_

#include<iostream>
#include<string>
#include<cstdlib>
#include<opencv2/opencv.hpp>
#include<sstream>

FILE *open_ffmpeg(const char *output, const char *codec, const char *res, const char *dst_res, const char *fps, const char *crf);
void write_ffmpeg(FILE *fptr, cv::Mat &frame);
void mux_audio(const char *output, const char *src, const char *final_file);
void close_stdout();
#endif
