
#include "ffmpeg_write.h"

#ifdef __APPLE__
#include<mach-o/dyld.h>
#include<libgen.h>
#include<limits.h>
#include<stdint.h>
#endif


#include<sstream>
#include<fstream>
#ifndef _WIN32
#include<unistd.h>
#endif
// ffmpeg -y -s 640x480  -pixel_format bgr24 -f rawvideo -r 30 -i pipe: -vcodec libx265 -pix_fmt yuv420p -crf 24 test.mp4
#include"ipc_client.hpp"
#ifdef __APPLE__
std::string ffmpeg_path = "ffmpeg";
#else
std::string ffmpeg_path = "ffmpeg";
#endif
extern void sendString(const std::string &s);

char buffer[1024*1024];
std::fstream file;
int stdout_save;

FILE *open_ffmpeg(const char *output, const char *codec, const char *res, const char *dst_res, const char *fps, const char *crf) {
    

    #ifdef __APPLE__

    char exePath[PATH_MAX];
    uint32_t size = sizeof(exePath);
    if (_NSGetExecutablePath(exePath, &size) != 0)
        ffmpeg_path="ffmpeg";

    // Get directory
    std::string dir = dirname(exePath);
    ffmpeg_path = dir + "/ffmpeg";;

    #endif

    std::string tag;
    std::string preset_opts;
    std::string codec_str(codec);
    
    if(codec_str=="libx265")
        tag = "-tag:v hvc1";
    
    if(codec_str=="h264_nvenc") {
        preset_opts = "-preset fast -rc vbr -cq ";
        preset_opts += crf;
    } else if(codec_str=="hevc_nvenc") {
        preset_opts = "-preset fast -rc vbr -cq ";
        preset_opts += crf;
        tag = "-tag:v hvc1";
    } else if(codec_str=="av1_nvenc") {
        preset_opts = "-preset fast -rc vbr -cq ";
        preset_opts += crf;
    } else if(codec_str=="h264_videotoolbox") {
        preset_opts = "-q:v ";
        preset_opts += crf;
    } else if(codec_str=="hevc_videotoolbox") {
        preset_opts = "-q:v ";
        preset_opts += crf;
        tag = "-tag:v hvc1";
    } else if(codec_str=="h264_qsv") {
        preset_opts = "-q ";
        preset_opts += crf;
    } else if(codec_str=="hevc_qsv") {
        preset_opts = "-q ";
        preset_opts += crf;
        tag = "-tag:v hvc1";
    } else if(codec_str=="av1_qsv") {
        preset_opts = "-q ";
        preset_opts += crf;
    } else if(codec_str=="h264_amf") {
        preset_opts = "-qp_p ";
        preset_opts += crf;
    } else if(codec_str=="hevc_amf") {
        preset_opts = "-qp_p ";
        preset_opts += crf;
        tag = "-tag:v hvc1";
    } else if(codec_str=="h264_cuda" || codec_str=="hevc_cuda") {
        preset_opts = "-cq ";
        preset_opts += crf;
        if(codec_str=="hevc_cuda") tag = "-tag:v hvc1";
    } else if(codec_str=="h264_dxva2" || codec_str=="hevc_d3d11va") {
        preset_opts = "-crf ";
        preset_opts += crf;
        if(codec_str=="hevc_d3d11va") tag = "-tag:v hvc1";
    } else {
        preset_opts = "-crf ";
        preset_opts += crf;
    }
    
    std::ostringstream stream;
    stream << ffmpeg_path << " -y -s " << dst_res << " -pixel_format bgr24 -f rawvideo -r " << fps << " -i pipe: -vcodec " << codec << " -pix_fmt yuv420p " << preset_opts << " " << tag << " " << output;
    
    std::cout<<"acidcam: " << stream.str() << "\n";
    
#ifndef _WIN32
    FILE *fptr = popen(stream.str().c_str(), "w");
#else
    FILE *fptr = _popen(stream.str().c_str(), "wb");
#endif
    if(!fptr) {
        std::cerr << "Error: could not open ffmpeg\n";
        return 0;
    }
    return fptr;
}

//ffmpeg -f avfoundation -i ":iShowU Audio Capture" -acodec libmp3lame -ab 128k -f mp3 -

void list_devices() {
#ifdef __APPLE__
    std::string s = ffmpeg_path + " -list_devices true -f avfoundation -i dummy";
    FILE *fptr = popen(s.c_str(), "r");
    if(!fptr) {
        std::cout << "acidcam: Error: could not read file...\n";
        exit(0);
    }
    while(!feof(fptr)) {
        char buf[1024];
        fgets(buf, 1024, fptr);
        std::cout << buf;
    }
    pclose(fptr);
#endif
}

void write_ffmpeg(FILE *fptr, cv::Mat &frame) {
    fwrite(frame.ptr(), sizeof(char), frame.total()*frame.elemSize(), fptr);
}

void close_stdout() {
    
}

void mux_audio(const char *output, const char *src, const char *final_file) {
    std::ostringstream stream;
    stream << ffmpeg_path << " -y -i \"" << output << "\" -i \"" << src << "\" -c copy -map 0:v:0 -map 1:a:0? -shortest \"" << final_file << "\"";
    std::cout << "acidcam: " << stream.str() << "\n";
#ifndef _WIN32
    FILE *fptr = popen(stream.str().c_str(), "r");
#else
    FILE *fptr = _popen(stream.str().c_str(), "r");
#endif
    if(!fptr) {
        std::cerr << "Error: could not open ffmpeg\n";
        return;
    }
#ifndef _WIN32
    pclose(fptr);
#else
    _pclose(fptr);
#endif
}

void rotate_90(const char *output, const char *src) {
    std::ostringstream stream;
    stream << ffmpeg_path << " -i \"" << output << "\" -c copy  -metadata:s:v:0 rotate=90 " << "\"" << src << "\"";
    std::cout << "acidcam: " << stream.str() << "\n";
#ifndef _WIN32
    FILE *fptr = popen(stream.str().c_str(), "r");
#else
    FILE *fptr = _popen(stream.str().c_str(), "r");
#endif
    if(!fptr) {
        std::cerr << "Error: could not open ffmpeg\n";
        return;
    }
#ifndef _WIN32
    pclose(fptr);
#else
    _pclose(fptr);
#endif
    
}
