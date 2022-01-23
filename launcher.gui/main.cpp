#include"main_window.hpp"
#include<QApplication>
#include<QThread>
#ifndef _WIN32
#include<unistd.h>
#endif

extern QThread *tv;
extern bool sock_running;
extern int sockfd;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow window1;
    window1.show();
    app.exec();
#ifndef _WIN32
    sock_running = false;
    close(sockfd);
    tv->exit(0);
#endif
    return 0;
}
