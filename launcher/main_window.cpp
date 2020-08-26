
#include"main_window.hpp"
#include<cstdio>
#include<iostream>
#include<string>
#include<QTextStream>
#include<QApplication>
#include<QDir>
#include<fstream>
#include<thread>
#include<mutex>
#include<iomanip>
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
#include<sstream>
#include<QProcess>
#include<QtConcurrent>
#include<QThread>
#include<QLabel>
MainWindow *main_window_ptr;

#ifndef _WIN32

const char *SOCK_PATH = "/tmp/testsock";
const int BUF_SIZE=256;

void error(const std::string &text, bool err_no = true);
ServerThread *tv;
QThread *threadx;
bool sock_running = false;
int sockfd;

int sock_server() {
    sockaddr_un addr;
    int clientfd;
    tv->Log("\nlauncher: IPC Server Starting...\n");
    char buf[BUF_SIZE];
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sockfd == -1)
        error("socket");
    
    if(remove(SOCK_PATH) == -1 && errno != ENOENT)
        error("remove");
    
    memset(&addr, 0, sizeof(sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);
    
    
    if(bind(sockfd, (sockaddr *)&addr, sizeof(sockaddr_un)) == -1)
        error("bind");
    
    
    if(listen(sockfd, 5) == -1)
        error("listen");
                     
    sock_running = true;
    
    ssize_t bytesRead;
    while(sock_running) {
        clientfd = accept(sockfd, 0, 0);
        if(clientfd == -1)
            error("accept");
        
        while((bytesRead = read(clientfd, buf, BUF_SIZE)) > 0) {
            if(write(STDOUT_FILENO, buf, bytesRead) != bytesRead)
                error("partial read/write", false);

            buf[bytesRead] = 0;
            QString text = buf;
            tv->Log(buf);
        }
        
        if(bytesRead == -1) {
            error("read");
        }
        
        if(close(clientfd) == -1)
            error("close");
    }
    return EXIT_SUCCESS;
}

void error(const std::string &text, bool err_no) {
    std::cerr << "Error: " << text << "\n";
    if(err_no) std::cerr << strerror(errno) << "\n";
}

#endif


ServerThread::ServerThread() {
    
}

void ServerThread::process() {
#ifndef _WIN32
    sock_server();
#endif
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(1280, 1280);
    setWindowTitle("acidcamGL - Start New Session");
    command_stdout = new QTextEdit("acidcamGL Launcher - written by Jared Bruni", this);
    command_stdout->setGeometry(5, 1280/2, 1280-10, 1280/2-5);
    command = new QLineEdit("", this);
    command->setGeometry(5, 10, 1280-110, 30);
    command_stdout->setReadOnly(true);
    QString style_info = "font-size: 16px; font-family: monaco;";
    command->setStyleSheet(style_info);
    QLabel *temp1 = new QLabel(tr("Mode: "), this);
    temp1->setGeometry(15, 60, 50, 25);
    temp1->setStyleSheet(style_info);
    mode_select = new QComboBox(this);
    mode_select->setStyleSheet(style_info);
    mode_select->setGeometry(60, 60, 200, 25);
    mode_select->addItem(tr("Capture Device"));
    mode_select->addItem(tr("Video File"));
    connect(mode_select, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChanged_mode(int)));
    QLabel *temp2;
    temp2 = new QLabel(tr("Device Index: "), this);
    temp2->setStyleSheet(style_info);
    temp2->setGeometry(270, 60, 140, 25);
    device_edit = new QLineEdit("0", this);
    device_edit->setStyleSheet(style_info);
    device_edit->setGeometry(270+140+10, 60, 100, 25);
    connect(device_edit, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    updateCommand();
    start_button = new QPushButton(tr("Launch"), this);
    start_button->setGeometry(1280-100, 10, 90, 30);
    connect(start_button, SIGNAL(clicked()), this, SLOT(launchProgram()));
    command_stdout->setStyleSheet("background-color: black; color: white; font-size: 20px; font-family: monaco; ");
    tv = new ServerThread();
    threadx = new QThread;
    tv->moveToThread(threadx);
    connect(tv, SIGNAL(Log(const QString &)), this, SLOT(Log(const QString &)));
    connect(threadx, SIGNAL(started()), tv, SLOT(process()));
    threadx->start();
}

void MainWindow::launchProgram() {
    Log(tr("\nacidcamGL Launcher - Executing ...\n"));
    std::string value = command->text().toStdString();
    QString pwd("");
    char * PWD;
    PWD = getenv ("PWD");
    pwd.append(PWD);
    QString buf = pwd;
    QString tvalue;
    QTextStream stream(&tvalue);
    stream << "launcher: executing shell command: " << value.c_str() << "\n";
    QString program = "open";
    QStringList arguments;
#ifdef __APPLE__
    arguments << QString(pwd+"/"+"acidcamGL.app");
    arguments << "--args";
    QStringList pieces = QString(value.c_str()).split(" ", Qt::KeepEmptyParts);
    for(int i = 0; i < pieces.size(); ++i) {
        QString value = pieces.at(i);
        arguments << value;
    }
#endif
    Log(tvalue);
    tvalue = "";
    QProcess *myProcess = new QProcess();
    myProcess->setWorkingDirectory(pwd);
    myProcess->start(program, arguments);
    myProcess->waitForFinished();
    Log(myProcess->readAllStandardOutput());
    stream << "launcher: process executed with error code: " << myProcess->exitCode();
    Log(tvalue);
}

void MainWindow::Log(const QString &text) {
    QString t = command_stdout->toPlainText();
    t += tr(text.toStdString().c_str());
    command_stdout->setPlainText(t);
    QTextCursor tmpCursor = command_stdout->textCursor();
    tmpCursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    command_stdout->setTextCursor(tmpCursor);
}

void MainWindow::LogMessage(const QString &text) {
    Log(text);
    emit LogString(text);
}

void MainWindow::textChanged_device(const QString &) {
    updateCommand();
}

void MainWindow::comboChanged_mode(int) {
    updateCommand();
}
void MainWindow::updateCommand() {
    char * PWD;
    QString pwd;
    PWD = getenv ("PWD");
    pwd.append(PWD);
    QString buf;
    buf += "-g -p ";
    buf += QString(pwd+"/filters");
    buf += " -P";
    if(mode_select->currentIndex() == 0) {
        int value = atoi(device_edit->text().toStdString().c_str());
        if(value >= 0) {
            buf += " -d ";
            buf += device_edit->text();
        }
    }
    command->setText(buf);
}
