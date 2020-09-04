
#include "ffmpeg_write.h"
#include<sstream>
#include<fstream>
#ifndef _WIN32
#include<unistd.h>
#endif
// ffmpeg -y -s 640x480  -pixel_format bgr24 -f rawvideo -r 30 -i pipe: -vcodec libx265 -pix_fmt yuv420p -crf 24 test.mp4
#include"ipc_client.hpp"
#ifdef __APPLE__
std::string ffmpeg_path = "/Applications/acidcamGL/acidcamGL.app/Contents/MacOS/ffmpeg";
#else
std::string ffmpeg_path = "ffmpeg";
#endif
extern void sendString(const std::string &s);

/*
int in, out;

static bool start_subprocess(const char *command, int *pid, FILE **infd, FILE **outfd) {
    int p1[2], p2[2];
    if (!pid || !infd || !outfd)
        return false;

    if (pipe(p1) == -1)
        goto err_pipe1;
    if (pipe(p2) == -1)
        goto err_pipe2;
    if ((*pid = fork()) == -1)
        goto err_fork;

    if (*pid) {
        in = p1[1], out = p2[0];
        *infd = fdopen(in, "r");
        *outfd = fdopen(out, "w");
        close(p1[0]);
        close(p2[1]);
        return true;
    } else {
        dup2(p1[0], 0);
        dup2(p2[1], 1);
        close(p1[0]);
        close(p1[1]);
        close(p2[0]);
        close(p2[1]);
        execlp("/bin/sh","/bin/sh", "-c", command, (char *)NULL);
        fprintf(stderr, "error running %s: %s", command, strerror(errno));
        abort();
    }

err_fork:
    close(p2[1]);
    close(p2[0]);
err_pipe2:
    close(p1[1]);
    close(p1[0]);
err_pipe1:
    return false;
}
*/
char buffer[1024*1024];
std::fstream file;
int stdout_save;

FILE *open_ffmpeg(const char *output, const char *codec, const char *res, const char *dst_res, const char *fps, const char *crf) {
    
    std::string tag;
    if(std::string(codec)=="libx265")
        tag = "-tag:v hvc1";
    
    std::ostringstream stream;
    stream << ffmpeg_path << " -fflags discardcorrupt -y -s " << dst_res << " -pixel_format bgr24 " << " -pix_fmt yuv420p -f rawvideo -r " << fps << " -i pipe: -vcodec " << codec << " -pix_fmt yuv420p " <<  tag << " -crf " << crf << " " <<  output;
    
    std::cout<<"acidcam: " << stream.str() << "\n";
#ifndef _WIN32
    FILE *fptr = popen(stream.str().c_str(), "w");
#else
    FILE* fptr = _popen(stream.str().c_str(), "w");
#endif
    if(!fptr) {
        std::cerr << "Error: could not open ffmpeg\n";
        return 0;
    }
    
    return fptr;
    return 0;
}


void write_ffmpeg(FILE *fptr, cv::Mat &frame) {
    int bytes = fwrite(frame.ptr(), sizeof(char), frame.step[0] * frame.rows, fptr);
}

void close_stdout() {
    
}

void mux_audio(const char *output, const char *src, const char *final_file) {
    std::ostringstream stream;
    stream << ffmpeg_path << " -y -i " << output << " -i " << src << " -c copy -map 0:v:0 -map 1:a:0? -shortest " << final_file;
    std::cout << "acidcam: " << stream.str() << "\n";
#ifndef _WIN32
    FILE* fptr = popen(stream.str().c_str(), "r");
#else
    FILE* fptr = _popen(stream.str().c_str(), "r");
#endif
    if(!fptr) {
        std::cerr << "Error: could not open ffmpeg\n";
        return;
    }
    while(!feof(fptr)) {
        char buf[256];
        fgets(buf, 256, fptr);
        std::cout << buf;
#ifdef SYPHON_SERVER
        sendString(buf);
#endif
    }
#ifndef _WIN32
    pclose(fptr);
#else
    _pclose(fptr);
#endif
}
