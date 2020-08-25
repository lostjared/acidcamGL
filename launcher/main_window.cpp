
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
#endif
#include<sstream>
#include<QProcess>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
     setFixedSize(1280, 1280);
    setWindowTitle("acidcamGL - Start New Session");
    command_stdout = new QTextEdit("acidcamGL Launcher - written by Jared Bruni", this);
    command_stdout->setGeometry(5, 1280/2, 1280-10, 1280/2-5);
    command = new QLineEdit("", this);
    command->setGeometry(5, 10, 1280-110, 30);
    command_stdout->setReadOnly(true);
    char * PWD;
    QString pwd;
    PWD = getenv ("PWD");
    pwd.append(PWD);
    QString buf = pwd;
    buf += "/acidcamGL ";
    command->setText(buf);
    start_button = new QPushButton(tr("Launch"), this);
    start_button->setGeometry(1280-100, 10, 90, 30);
    connect(start_button, SIGNAL(clicked()), this, SLOT(launchProgram()));
    command_stdout->setStyleSheet("background-color: black; color: white; font-size: 18px; ");
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
        Log("launcher: Couldn't open file for writing...\n");
        return;
    }
    file << "#/bin/sh\n\n";
    file << value << "\n";
    file.close();
    
    QString program = "open";
    QStringList arguments;
    arguments << "/Users/jared/Source/newestac2/acidcamGL/launcher/acidcam";

    QProcess *myProcess = new QProcess();
    myProcess->start(program, arguments);
    myProcess->waitForFinished();
    Log(myProcess->readAllStandardOutput());
    QString tvalue;
    QTextStream stream(&tvalue);
    stream << "launcher: exited with code: " << myProcess->exitCode();
    Log(tvalue);
}

void MainWindow::Log(const QString &text) {
    QString t = command_stdout->toPlainText();
    t += text;
    command_stdout->setPlainText(t);
    QTextCursor tmpCursor = command_stdout->textCursor();
    tmpCursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    command_stdout->setTextCursor(tmpCursor);
}
