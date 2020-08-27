
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

QString application_path = "/Applications/acidcamGL";

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
    select_temp->setGeometry(15, 60+25+15, 140, 20);
    QString pwd = QDir().currentPath();
#ifdef __APPLE__
    pwd = application_path;
    std::string f = pwd.toStdString();
    auto pos = f.rfind("launcher");
    if(pos != std::string::npos) {
        f = f.substr(0, pos);
        pwd = f.c_str();
    }
#endif
    select_filters_text = new QLineEdit(pwd+"/filters", this);
    select_filters_text->setStyleSheet(style_info);
    select_filters_text->setGeometry(15+150+10, 60+25+10, 250, 30);
    select_filters = new QPushButton(tr("Select"), this);
    select_filters->setStyleSheet(style_info);
    select_filters->setGeometry(15+140+10+250+20, 60+25+10,100,30);
    select_video = new QPushButton(tr("Select"), this);
    QLabel *select_temp1 = new QLabel(tr("Select Video: "), this);
    select_temp1->setGeometry(5+15+140+10+250+20+60+25+10+5, 60, 125, 20);
    select_video->setStyleSheet(style_info);
    select_video->setGeometry(5+15+140+10+250+20+60+25+10+5+125+5+150+5, 60, 100, 30);
    select_temp1->setStyleSheet(style_info);
    select_video_text = new QLineEdit("", this);
    select_video_text->setStyleSheet(style_info);
    select_video_text->setGeometry(5+15+140+10+250+20+60+25+10+5+125+5, 60, 150, 30);
    connect(device_edit, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    start_button = new QPushButton(tr("Launch"), this);
    start_button->setGeometry(1280-100, 10, 90, 30);
    start_button->setStyleSheet(style_info);
    connect(start_button, SIGNAL(clicked()), this, SLOT(launchProgram()));
    command_stdout->setStyleSheet("background-color: black; color: white; font-size: 20px; font-family: monaco; ");
    tv = new ServerThread();
    threadx = new QThread;
    tv->moveToThread(threadx);
    connect(tv, SIGNAL(Log(const QString &)), this, SLOT(Log(const QString &)));
    connect(threadx, SIGNAL(started()), tv, SLOT(process()));
    connect(select_filters, SIGNAL(clicked()), this, SLOT(selectShaders()));
    connect(select_video, SIGNAL(clicked()),this, SLOT(selectVideo()));
    threadx->start();
    QLabel *select_temp2 = new QLabel(tr("Set Path: "), this);
    select_temp2->setStyleSheet(style_info);
    select_temp2->setGeometry(5+15+140+10+250+20+60+25+10+5, 60+25+10,100,30);
    select_path = new QPushButton(tr("Select"), this);
    select_path->setStyleSheet(style_info);
    select_path->setGeometry(10+5+15+10+250+125+20+60+25+10+5+125+5+150+5+5, 60+25+10, 100, 30);
    select_temp1->setStyleSheet(style_info);
    select_path_text = new QLineEdit("", this);
    select_path_text->setStyleSheet(style_info);
    select_path_text->setGeometry(10+5+15+10+125+250+20+60+25+10+5+125+5+5, 60+25+10, 150, 25);
    connect(select_path_text, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    connect(select_video_text, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    connect(select_path, SIGNAL(clicked()), this, SLOT(selectPath()));
    QString homeLocation = QStandardPaths::locate(QStandardPaths::PicturesLocation, QString(), QStandardPaths::LocateDirectory);
    select_path_text->setText(homeLocation+"acidcamGL_Snapshot");
    QLabel *camera_temp = new QLabel(tr("Camera Resolution: "), this);
    camera_temp->setStyleSheet(style_info);
    camera_temp->setGeometry(15, 60+25+10+40, 200, 25);
    camera_res = new QLineEdit(tr("1280x720"), this);
    camera_res->setStyleSheet(style_info);
    camera_res->setGeometry(215, 60+25+10+40, 150, 30);
    connect(camera_res, SIGNAL(editingFinished()), this, SLOT(updateCommand()));

    QLabel *camera_temp1 = new QLabel(tr("Window Resolution: "), this);
    camera_temp1->setStyleSheet(style_info);
    camera_temp1->setGeometry(215+150+10+15, 60+25+10+40, 200, 25);
    window_res = new QLineEdit(tr("1280x720"), this);
    window_res->setStyleSheet(style_info);
    window_res->setGeometry(215+150+10+15+200, 60+25+10+40, 150, 30);
    connect(window_res, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    full_screen = new QCheckBox(tr("Full Screen"), this);
    full_screen_resize = new QCheckBox(tr("Resize"), this);
    full_screen->setStyleSheet(style_info);
    full_screen_resize->setStyleSheet(style_info);
    full_screen->setGeometry(215+150+10+15+200+150+10, 60+25+10+40, 150, 25);
    full_screen_resize->setGeometry(215+150+10+15+200+150+10+150, 60+25+10+40, 150, 25);
    monitor_ = new QLineEdit(tr("0"), this);
    monitor_->setStyleSheet(style_info);
    QLabel *mon_text = new QLabel(tr("Monitor: "), this);
    mon_text->setStyleSheet(style_info);
    mon_text->setGeometry(215+150+10+15+200+150+10+150+50+10+50, 60+25+10+40, 100, 30);
    monitor_->setGeometry(215+150+10+15+200+150+10+150+150+10+10+50, 60+25+10+40, 100, 30);
    command_stdout->setReadOnly(true);
    command->setReadOnly(true);
    connect(full_screen, SIGNAL(clicked()), this, SLOT(updateCommand()));
    connect(full_screen_resize, SIGNAL(clicked()), this, SLOT(updateCommand()));
    //Log(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory));
    connect(monitor_, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    updateCommand();
}

void MainWindow::launchProgram() {
    Log(tr("\nacidcamGL Launcher - Executing ...\n"));
    std::string value = command->text().toStdString();
    QString pwd("");
    QString buf = pwd;
    QString tvalue;
    QTextStream stream(&tvalue);
    stream << "launcher: executing shell command: " << value.c_str() << "\n";
    QString program;
    QStringList arguments;
#ifdef __APPLE__
    program = "open";
    pwd = application_path;
    std::string f = pwd.toStdString();
    auto pos = f.rfind("launcher");
    if(pos != std::string::npos) {
        f = f.substr(0, pos-1);
        pwd = f.c_str();
    }
    Log(pwd);
    arguments << QString(pwd+"/"+"acidcamGL.app");
    arguments << "--args";
    arguments << cmd_list;
#else
    program = QString(pwd+"/launcher.exe");
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

void MainWindow::selectPath() {

    QString dir = QFileDialog::getExistingDirectory(this, tr("Select Shaders Directory"),
                                                 "~",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if(dir.length()>0)
        select_path_text->setText(dir);
    updateCommand();
}

void MainWindow::selectVideo() {
    QString name = QFileDialog::getOpenFileName(this,
        tr("Open Video"), "/Users", tr("Image Files (*.mov *.mp4 *.mkv *.avi *.m4v)"));
    select_video_text->setText(name);
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
    QString temp = "";
    temp += select_filters_text->text();
    cmd_list << temp;
    if(mode_select->currentIndex() == 0) {
        int value = atoi(device_edit->text().toStdString().c_str());
        if(value >= 0) {
            cmd_list << "-d";
            cmd_list << device_edit->text();
        }
        std::string vf = camera_res->text().toStdString();
        if(vf.find("x") != std::string::npos) {
            std::string left = vf.substr(0, vf.find("x"));
            std::string right = vf.substr(vf.find("x")+1, vf.length());
            if(atoi(left.c_str()) >= 320 && atoi(right.c_str()) >= 240) {
                cmd_list << "-c";
                cmd_list << camera_res->text();
            }
        }
    } else {
         if(select_video_text->text().length()>0) {
            cmd_list << "-i";
            cmd_list << select_video_text->text();
        }
    }

    if(select_path_text->text().length()>0) {
        cmd_list << "-e";
        cmd_list << select_path_text->text();
    }
    std::string vf = window_res->text().toStdString();
    if(vf.find("x") != std::string::npos) {
        std::string left = vf.substr(0, vf.find("x"));
        std::string right = vf.substr(vf.find("x")+1, vf.length());
        if(atoi(left.c_str()) >= 320 && atoi(right.c_str()) >= 240) {
            cmd_list << "-r";
            cmd_list << window_res->text();
        }
    }

    if(full_screen->isChecked()) {
        if(full_screen_resize->isChecked()) {
            cmd_list << "-f";
        } else {
            cmd_list << "-F";
        }
        cmd_list << "-M";
        cmd_list << monitor_->text();
    }

    QString buf;
    for(int i = 0; i < cmd_list.size(); ++i) {
        buf += cmd_list.at(i) + " ";
    }
    command->setText(buf);
}
