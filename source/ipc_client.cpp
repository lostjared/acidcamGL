#ifndef _WIN32
#include "ipc_client.hpp"
const char *SOCK_PATH = "/tmp/testsock";
const int BUF_SIZE=256;


void error(const std::string &text, bool err_no = true);

int sockfd;

int client_main() {
    sockaddr_un addr;
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1)
        error("socket");
    
    memset(&addr, 0, sizeof(sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path) -1);
    
    if(connect(sockfd, (sockaddr *)&addr, sizeof(sockaddr_un)) == -1)
        error("connect");
    return 0;
}

void sendString(const std::string &text) {
    if(sockfd > 0) {
        ssize_t bytesRead;
        char buf[BUF_SIZE];
        if(write(sockfd, text.c_str(), text.length()) != text.length())
            error("partial write", false);
        
    }
}

void error(const std::string &text, bool err_no) {
    std::cerr << "Error: " << text << "\n";
    if(err_no) std::cerr << strerror(errno) << "\n";
}
#endif

