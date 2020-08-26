
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
    bool enable;
public:
    CoutRedirect(bool e) {
        enable = e;
        if(enable)
            old = std::cout.rdbuf( buffer.rdbuf() );
    }
    
    void setEnabled(bool e) {
        enable = e;
        enable = e;
        if(enable)
            old = std::cout.rdbuf( buffer.rdbuf() );
        else
            std::cout.rdbuf( old );
    }
    
    std::string getString() {
        if(enable) {
            std::string val = buffer.str();
            buffer.str("");
            return val;
        } else return "";
    }
    
    ~CoutRedirect( ) {
        if(enable)
            std::cout.rdbuf( old );
    }
    
private:
    std::stringstream buffer;
    std::streambuf * old;
};

extern CoutRedirect redirect;

#endif
#endif

