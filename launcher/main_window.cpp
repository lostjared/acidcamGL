
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
#include<sstream>
#include<QProcess>
#include<QtConcurrent>
#include<QThread>
#include<QLabel>
#include<QFileDialog>

QThread *threadx;
ServerThread *tv;

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

    QLabel *select_temp = new QLabel(tr("Select Shaders:"), this);
    select_temp->setStyleSheet(style_info);
    select_temp->setGeometry(15, 60+25+10, 140, 20);

    QString pwd = QDir().currentPath();

#ifdef __APPLE__
    std::string f = pwd.toStdString();
    auto pos = f.rfind("launcher");
    if(pos != std::string::npos) {
        f = f.substr(0, pos);
        pwd = f.c_str();
    }
#endif


    select_filters_text = new QLineEdit(pwd+"filters", this);
    select_filters_text->setStyleSheet(style_info);
    select_filters_text->setGeometry(15+140+10, 60+25+10, 250, 30);
    select_filters = new QPushButton(tr("Select"), this);
    select_filters->setStyleSheet(style_info);
    select_filters->setGeometry(15+140+10+250+10, 60+25+10,100,30);
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
    connect(select_filters, SIGNAL(clicked()), this, SLOT(selectShaders()));
    threadx->start();
}

void MainWindow::launchProgram() {
    Log(tr("\nacidcamGL Launcher - Executing ...\n"));
    std::string value = command->text().toStdString();
    QString pwd("");
    QString buf = pwd;
    QString tvalue;
    QTextStream stream(&tvalue);
    stream << "launcher: executing shell command: " << value.c_str() << "\n";
    QString program = "open";
    QStringList arguments;
#ifdef __APPLE__
    pwd = QDir().currentPath();
    std::string f = pwd.toStdString();
    auto pos = f.rfind("launcher");
    if(pos != std::string::npos) {
        f = f.substr(0, pos);
        pwd = f.c_str();
    }
    arguments << QString(pwd+"/"+"acidcamGL.app");
    arguments << "--args";
    arguments << cmd_list;
#endif
    Log(tvalue);
    tvalue = "";
    QProcess *myProcess = new QProcess();
    myProcess->setWorkingDirectory(pwd);
    myProcess->start(program, arguments);
    myProcess->waitForFinished();
    myProcess->readAllStandardOutput();
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

void MainWindow::selectShaders() {
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Shaders Directory"),
                                                 "~",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(dir.length()>0)
        select_filters_text->setText(dir);
    updateCommand();
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
    cmd_list.clear();
    cmd_list << "-P";
    cmd_list << "-g";
    cmd_list << "-p";
    cmd_list << select_filters_text->text();

    if(mode_select->currentIndex() == 0) {
        int value = atoi(device_edit->text().toStdString().c_str());
        if(value >= 0) {
            cmd_list << "-d";
            cmd_list << device_edit->text();
        }
    }
    QString buf;
    for(int i = 0; i < cmd_list.size(); ++i) {
        buf += cmd_list.at(i) + " ";
    }
    command->setText(buf);
}
