#include"main_window.hpp"
#include<QApplication>
#include<QFile>
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
    QFile styleFile(":/stylesheet.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString style = QLatin1String(styleFile.readAll());
        app.setStyle("Fusion");
        app.setStyleSheet(style);
        styleFile.close();
    }
    window1.show();
    app.exec();
#ifndef _WIN32
    sock_running = false;
    close(sockfd);
    tv->exit(0);
#endif
    return 0;
}
