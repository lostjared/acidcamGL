
#include "ffmpeg_write.h"
#include<sstream>
// ffmpeg -y -s 640x480  -pixel_format bgr24 -f rawvideo -r 30 -i pipe: -vcodec libx265 -pix_fmt yuv420p -crf 24 test.mp4


FILE *open_ffmpeg(const char *output, const char *res, const char *dst_res, const char *fps, const char *crf) {
    
    std::ostringstream stream;
    stream << "ffmpeg -y -s " << dst_res << " -pixel_format bgr24 -f rawvideo -r " << fps << " -i pipe: -vcodec libx265 -pix_fmt yuv420p " << " -tag:v hvc1 -crf " << crf << " " <<  output;
    
    std::cout<<"acidcam: " << stream.str() << "\n";
    FILE *fptr = popen(stream.str().c_str(), "w");
    if(!fptr) {
        std::cerr << "Error: could not open ffmpeg\n";
        return 0;
    }
    
    return fptr;
}

void write_ffmpeg(FILE *fptr, cv::Mat &frame) {
    fwrite(frame.ptr(), sizeof(char), frame.total()*frame.elemSize(), fptr);
}

void mux_audio(const char *output, const char *src, const char *final_file) {
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
    }
    pclose(fptr);
}
