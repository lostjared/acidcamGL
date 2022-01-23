#ifndef __SOCK_SERVER_H__
#define __SOCK_SERVER_H__
#include<QThread>
#ifndef _WIN32
#include<sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include<sys/un.h>
#include<sys/socket.h>
#include<string>
#include<cstring>
#include<errno.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<errno.h>
#endif
#include<iostream>

class ServerThread : public QThread {
    Q_OBJECT
public:
    ServerThread();
public slots:
    void process();
signals:
    void Log(const QString &text);
};

#endif

