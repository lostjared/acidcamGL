#include"main_window.hpp"
#include<QApplication>
#include<QThread>
#include<unistd.h>

extern QThread *tv;
extern bool sock_running;
extern int sockfd;

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow window1;
    window1.show();
    app.exec();
    sock_running = false;
    close(sockfd);
    tv->exit(0);
    return 0;
}
