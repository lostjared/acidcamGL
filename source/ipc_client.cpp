#ifndef _WIN32
#include "ipc_client.hpp"
const char *SOCK_PATH = "/tmp/testsock";
const int BUF_SIZE=256;


void error(const std::string &text, bool err_no = true);

int sockfd;

int client_main() {
    return 0;
}

void sendString(const std::string &text) {
    printf("%s", text.c_str());
    fflush(stdout);
    fflush(stdin);
}

void error(const std::string &text, bool err_no) {
    std::cerr << "Error: " << text << "\n";
    if(err_no) std::cout << strerror(errno) << "\n";
}
#endif

