
#include "ffmpeg_write.h"
#include<sstream>
#include<fstream>
#ifndef _WIN32
#include<unistd.h>
#endif
// ffmpeg -y -s 640x480  -pixel_format bgr24 -f rawvideo -r 30 -i pipe: -vcodec libx265 -pix_fmt yuv420p -crf 24 test.mp4

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
extern int redirect;

FILE *open_ffmpeg(const char *output, const char *codec, const char *res, const char *dst_res, const char *fps, const char *crf) {
    
#ifndef _WIN32
    std::string tag;
    if(std::string(codec)=="libx265")
        tag = "-tag:v hvc1";
    
    std::ostringstream stream;
    stream << "ffmpeg -y -s " << dst_res << " -pixel_format bgr24 -f rawvideo -r " << fps << " -i pipe: -vcodec " << codec << " -pix_fmt yuv420p " <<  tag << " -crf " << crf << " " <<  output;
    
    std::cout<<"acidcam: " << stream.str() << "\n";
    
    
    fflush(stdout); //clean everything first
    stdout_save = dup(STDOUT_FILENO); //save the stdout state
    setvbuf(stdout, buffer, _IOFBF, 1024); //set buffer to stdout
        
    stream << " 2>&1 | tee /Applications/acidcamGL/stdout";
    
    FILE *fptr = popen(stream.str().c_str(), "w");
#ifdef SYPHON_SERVER
    freopen("/Applications/acidcamGL/stdout", "a", stdout); //redirect stdout to null pointer
#endif
    
    if(!fptr) {
        std::cerr << "Error: could not open ffmpeg\n";
        return 0;
    }
    
    return fptr;
#endif
    return 0;
}


void write_ffmpeg(FILE *fptr, cv::Mat &frame) {
#ifndef _WIN32
    fwrite(frame.ptr(), sizeof(char), frame.total()*frame.elemSize(), fptr);
    std::cout << buffer;
#ifdef SYPHON_SERVER
    if(redirect != 0) {
        sendString(buffer);
    }
#endif
#endif
}

void close_stdout() {
#ifndef SYPHON_SERVER
    freopen("/Applications/acidcamGL/stdout", "a", stdout);
    dup2(stdout_save, STDOUT_FILENO);
    setvbuf(stdout, NULL, _IONBF, 1024);
#endif
}

void mux_audio(const char *output, const char *src, const char *final_file) {
#ifndef _WIN32
    std::ostringstream stream;
    stream << "ffmpeg -y -i " << output << " -i " << src << " -c copy -map 0:v:0 -map 1:a:0? -shortest " << final_file;
    std::cout << "acidcam: " << stream.str() << "\n";
    FILE *fptr = popen(stream.str().c_str(), "r");
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
    pclose(fptr);
#endif
}
