
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
    setFixedSize(1280, 375+500+10);
    setWindowTitle("acidcamGL - Start New Session");
    command_stdout = new QTextEdit("acidcamGL Launcher - written by Jared Bruni", this);
    command_stdout->setGeometry(5, 375, 1280-10, 500);
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
#ifdef __APPLE__
    mode_select->addItem(tr("Screen Capture"));
#endif
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

    syphon_enabled = new QCheckBox(tr("Syphon Enabled"), this);
    syphon_enabled->setStyleSheet(style_info);
    syphon_enabled->setGeometry(5+15+140+10+250+20+60+25+10+5+125+5+150+100+10, 60, 200, 30);
    connect(syphon_enabled, SIGNAL(clicked()), this, SLOT(updateCommand()));
#ifndef __APPLE__
    syphon_enabled->hide();
#endif
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
    command->setReadOnly(false);
    connect(full_screen, SIGNAL(clicked()), this, SLOT(updateCommand()));
    connect(full_screen_resize, SIGNAL(clicked()), this, SLOT(updateCommand()));
    //Log(QStandardPaths::locate(QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory));
    
    QLabel *start_lbl = new QLabel(tr("Shader: "), this);
    start_lbl->setStyleSheet(style_info);
    start_lbl->setGeometry(20, 135+35, 100, 30);
    start_shader = new QLineEdit(tr("0"), this);
    start_shader->setGeometry(120, 135+35, 100, 30);
    start_shader->setStyleSheet(style_info);
    QLabel *start_filter_lbl = new QLabel(tr("Filter: "), this);
    start_filter_lbl->setGeometry(235, 135+35, 100, 30);
    start_filter_lbl->setStyleSheet(style_info);
    start_filter = new QLineEdit(tr("0"), this);
    start_filter->setGeometry(335, 135+35, 100, 30);
    start_filter->setStyleSheet(style_info);
    QLabel *start_sec_lbl = new QLabel(tr("Start At: "), this);
    start_sec_lbl->setGeometry(335+110, 135+35, 100, 30);
    start_sec_lbl->setStyleSheet(style_info);
    start_sec = new QLineEdit(tr("0"), this);
    start_sec->setStyleSheet(style_info);
    start_sec->setGeometry(335+210, 135+35, 100, 30);
    connect(monitor_, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    connect(start_shader, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    connect(start_filter, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    connect(start_sec, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    
    record_video = new QCheckBox(tr("Record"), this);
    record_video->setStyleSheet(style_info);
    record_video->setGeometry(20, 135+40+35, 100, 30);
    record_type = new QComboBox(this);
    record_type->setStyleSheet(style_info);
    record_type->setGeometry(125, 135+40+35, 100, 30);
    record_type->addItem("x264");
    record_type->addItem("x265");
    record_name = new QLineEdit(tr("output.mp4"), this);
    record_name->setStyleSheet(style_info);
    record_name->setGeometry(240, 135+40+35, 250, 30);
    QLabel *crf_lbl = new QLabel(tr("CRF"), this);
    crf_lbl->setStyleSheet(style_info);
    crf_lbl->setGeometry(240+260, 135+40+35, 100, 30);
    record_crf = new QLineEdit(tr("22"), this);
    record_crf->setStyleSheet(style_info);
    record_crf->setGeometry(240+260+40, 135+40+35, 100, 30);

    connect(record_crf, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    connect(record_name, SIGNAL(editingFinished()), this, SLOT(updateCommand()));

    connect(record_video, SIGNAL(clicked()), this, SLOT(updateCommand()));
    connect(record_type, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChanged_mode(int)));
    
    QLabel *mat_lbl = new QLabel(tr("Material: "), this);
    mat_lbl->setStyleSheet(style_info);
    mat_lbl->setGeometry(20, 135+40+35+35, 100, 30);
    
    material_filename = new QLineEdit(tr(""), this);
    material_filename->setStyleSheet(style_info);
    material_filename->setGeometry(125, 135+40+35+35, 200, 30);
    
    material_set = new QPushButton(tr("Select"), this);
    material_set->setStyleSheet(style_info);
    material_set->setGeometry(330, 135+40+35+35, 100, 30);
    
    connect(material_set, SIGNAL(clicked()), this, SLOT(setMatPath()));
    connect(material_filename, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
    
    QLabel *playlist_lbl = new QLabel(tr("Playlist: "), this);
    playlist_lbl->setStyleSheet(style_info);
    playlist_lbl->setGeometry(20, 135+40+35+35+35, 100, 30);
    
    playlist_file = new QLineEdit(tr(""), this);
    playlist_file->setStyleSheet(style_info);
    playlist_file->setGeometry(125, 135+40+35+35+35, 200, 30);
    
    playlist_set = new QPushButton(tr("Select"), this);
    playlist_set->setStyleSheet(style_info);
    playlist_set->setGeometry(330, 135+40+35+35+35, 100, 30);

    connect(playlist_file, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
  
    connect(playlist_set, SIGNAL(clicked()), this, SLOT(setPlaylistPath()));
    
    enable_playback = new QCheckBox(tr("Enable Playback"), this);
    enable_playback->setStyleSheet(style_info);
    enable_playback->setGeometry(440, 135+40+35+35+35,200, 30);
    
    QLabel *ebpm_lbl = new QLabel(tr("Beats per Minute: "), this);
    ebpm_lbl->setStyleSheet(style_info);
    ebpm_lbl->setGeometry(550+100, 135+40+35+35+35, 200, 30);
    
    enable_bpm = new QLineEdit(tr("60"), this);
    enable_bpm->setStyleSheet(style_info);
    enable_bpm->setGeometry(660+200, 135+40+35+35+35, 200, 30);

    connect(enable_bpm, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
  
    connect(enable_playback, SIGNAL(clicked()), this, SLOT(updateCommand()));
    
    QLabel *auto_lbl = new QLabel(tr("AutoFilter"), this);
    auto_lbl->setStyleSheet(style_info);
    auto_lbl->setGeometry(20, 135+40+35+35+35+35, 100, 30);
    
    auto_filter = new QLineEdit(tr(""), this);
    auto_filter->setStyleSheet(style_info);
    auto_filter->setGeometry(125, 135+40+35+35+35+35, 200, 30);
    
    auto_set = new QPushButton(tr("Select"), this);
    auto_set->setStyleSheet(style_info);
    auto_set->setGeometry(330, 135+40+35+35+35+35, 100, 30);

    connect(auto_filter, SIGNAL(editingFinished()), this, SLOT(updateCommand()));
  
    connect(auto_set, SIGNAL(clicked()), this, SLOT(setAutoFilter()));

    
    updateCommand();
}

void MainWindow::launchProgram() {
    Log(tr("\nacidcamGL Launcher - Executing ...\n"));
    std::string value = command->text().toStdString();
    QString tvalue;
    QTextStream stream(&tvalue);
    stream << "launcher: executing shell command: " << value.c_str() << "\n";
    QString program;
    QStringList arguments;
    Log(tvalue);
    tvalue = "";
    QString cmd_string;
#ifdef __APPLE__
    cmd_string = "/Applications/acidcamGL/acidcamGL.app/Contents/MacOS/acidcamGL ";
#else
    cmd_string = "acidcamGL ";
#endif
    cmd_string += command->text();
    
#if defined(__APPLE__) || defined(__linux__)
    FILE *fptr_ = popen(cmd_string.toStdString().c_str(), "r");
    if(!fptr_) {
        std::cerr << "Error could not launch process...\n";
    }
    fptr.push_back(fptr_);
#elif defined(_WIN32)
    std::thread e([=]() {
        system(cmd_string.toStdString().c_str());
    });
    e.detach();
#endif
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

void MainWindow::setMatPath() {
    QString name = QFileDialog::getOpenFileName(this,
        tr("Open Video/Image"), "/Users", tr("Image Files (*.mov *.mp4 *.mkv *.avi *.m4v *.jpg *.png *.bmp *.tif)"));
    material_filename->setText(name);
    updateCommand();
}

void MainWindow::setPlaylistPath() {
    QString name = QFileDialog::getOpenFileName(this,
        tr("Open Playlist"), "/Users", tr("Playlist Files (*.key)"));
    playlist_file->setText(name);
    updateCommand();
}

void MainWindow::setAutoFilter() {
    QString name = QFileDialog::getOpenFileName(this,
        tr("Open Autofilter"), "/Users", tr("Autofilter Files (*.af)"));
    auto_filter->setText(name);
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
    cmd_list << QString("\"") + temp + "\"";
    select_video->setEnabled(true);
    select_video_text->setEnabled(true);
    device_edit->setEnabled(true);
    if(mode_select->currentIndex() == 0) {
        select_video->setEnabled(false);
        select_video_text->setEnabled(false);
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
    } else if(mode_select->currentIndex()==1) {
        device_edit->setEnabled(false);
         if(select_video_text->text().length()>0) {
            cmd_list << "-i";
            cmd_list << QString("\"") + select_video_text->text() + "\"";
        }
    } else if(mode_select->currentIndex()==2) {
        cmd_list << "-G";
    }

    if(select_path_text->text().length()>0) {
        cmd_list << "-e";
        cmd_list << QString("\"") + select_path_text->text() + "\"";
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
    if(syphon_enabled->isChecked()) {
        cmd_list << "-Y";
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
    if(start_shader->text() != "0")
        cmd_list << "-H" << start_shader->text();
    if(start_filter->text() != "0")
        cmd_list << "-S" << start_filter->text();
    if(start_sec->text() != "0")
        cmd_list << "-7" << start_sec->text();
    
    if(record_video->isChecked()) {
        if(record_type->currentIndex() == 0) {
            cmd_list << "-4";
        } else {
            cmd_list << "-5";
        }
        cmd_list << "-o";
        cmd_list << record_name->text();
        cmd_list << "-m";
        cmd_list << record_crf->text();
    }
    
    if(material_filename->text() != "") {
        cmd_list << "-T";
        QString filename = QString("\"") + material_filename->text() + "\"";
        cmd_list << filename;
    }
    
    if(playlist_file->text() != "") {
        cmd_list << "-L" << (QString("\"") + playlist_file->text() + "\"");
    }
    
    if(enable_playback->isChecked() && playlist_file->text() != "") {
        cmd_list << "-B" << "-q" << "-w" << enable_bpm->text();
    }
    
    if(auto_filter->text() != "") {
        cmd_list << "-A" << QString("\"") + auto_filter->text() + "\"";
    }
    
    QString buf;
    for(int i = 0; i < cmd_list.size(); ++i) {
        buf += cmd_list.at(i) + " ";
    }
    
    command->setText(buf);
}
