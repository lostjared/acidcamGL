#include <iostream>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

void print_device_info(const char* device) {
    int fd = open(device, O_RDWR);
    if (fd == -1) {
        std::cerr << "Error opening device: " << device << std::endl;
        return;
    }

    struct v4l2_capability cap;
    if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
        std::cerr << "Error querying device capabilities: " << device << std::endl;
        close(fd);
        return;
    }

    std::cout << "Device: " << device << std::endl;
    std::cout << "Driver: " << cap.driver << std::endl;
    std::cout << "Card: " << cap.card << std::endl;
    std::cout << "Bus Info: " << cap.bus_info << std::endl;
    std::cout << "Version: " << ((cap.version >> 16) & 0xFF) << "."
              << ((cap.version >> 8) & 0xFF) << "."
              << (cap.version & 0xFF) << std::endl;
    std::cout << "Capabilities: 0x" << std::hex << cap.capabilities << std::dec << std::endl;
    std::cout << std::endl;

    close(fd);
}

int main(int argc, char **argv) {

    if(argc != 2) 
        return;
    const int max_devices = atoi(argv[1]);
    for (int i = 0; i < max_devices; ++i) {
        std::string device = "/dev/video" + std::to_string(i);
        print_device_info(device.c_str());
    }
    return 0;
}