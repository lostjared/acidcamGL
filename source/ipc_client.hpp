
#ifndef __SRC_CLI_H_
#define __SRC_CLI_H_
#ifndef _WIN32
#include<iostream>
#include<sys/socket.h>
#include<sys/un.h>
#include<unistd.h>
#include<cstring>
#include<errno.h>
#include<string>
#include<sstream>

void sendString(const std::string &text);
int client_main();

class CoutRedirect {

public:
    CoutRedirect() {
        old = std::cout.rdbuf( buffer.rdbuf() );
    }

    std::string getString() {
        std::string val = buffer.str();
        buffer.str("");
        return val;
    }

    ~CoutRedirect( ) {
        std::cout.rdbuf( old );
    }

private:
    std::stringstream buffer;
    std::streambuf * old;
};

extern CoutRedirect redirect;

#endif
#endif

