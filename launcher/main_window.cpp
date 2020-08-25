
#include"main_window.hpp"
#include<cstdio>
#include<iostream>
#include<string>
#include<QTextStream>
#include<QDir>
#include<fstream>

#ifndef _WIN32
#include<sys/stat.h>
#endif


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
     setFixedSize(1280, 1280);
    setWindowTitle("acidcamGL - Start New Session");
    command_stdout = new QTextEdit("acidcamGL Launcher - written by Jared Bruni", this);
    command_stdout->setGeometry(5, 1280/2, 1280-10, 1280/2-5);
    command = new QLineEdit("", this);
    command->setGeometry(5, 10, 1280-110, 30);
    command_stdout->setReadOnly(true);
    command->setText("acidcamGL ");
    start_button = new QPushButton(tr("Launch"), this);
    start_button->setGeometry(1280-100, 10, 90, 30);
    connect(start_button, SIGNAL(clicked()), this, SLOT(launchProgram()));
 }

void MainWindow::launchProgram() {

    Log("\nacidcamGL Launcher - Executing ...\n");
    std::string value = command->text().toStdString();
    QString pwd("");
    char * PWD;
    PWD = getenv ("PWD");
    pwd.append(PWD);
    QString buf = pwd;
    
    std::fstream file;
    file.open(QString(buf+"/acidcam").toStdString().c_str(), std::ios::out);
    if(!file.is_open()) {
        Log("launcher: Couldn't open file for writing...");
        return;
    }
    file << "#/bin/sh\n\n";
    file << value << "\n";
    file.close();
    //chmod(QString(buf+"/acidcam").toStdString().c_str(), 777);
#ifdef __APPLE__
    QString cmd = "open ";
    cmd += buf;
    cmd += "/acidcam";
    int code = std::system(cmd.toStdString().c_str());
    QString txt;
    QTextStream stream(&txt);
    stream << "launcher: Program command: " << cmd << " exited with code: " << code << "\n";
    Log(txt);
#else
    
#endif
    
}

void MainWindow::Log(const QString &text) {
    QString t = command_stdout->toPlainText();
    t += text;
    command_stdout->setPlainText(t);
}
