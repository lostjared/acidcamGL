
#include"auto_window.hpp"
#include<fstream>
#include<QFileDialog>


Auto::Auto(QWidget *parent) : QDialog(parent) {
    setFixedSize(640, 480);
    setWindowTitle("AutoFilter - Editor");
    setWindowIcon(QPixmap(":/images/icon.png"));
    createControls();
}

void Auto::createControls() {
    
    QString style_info = "font-size: 16px; font-family: monaco;";
    
    box = new QListWidget(this);
    box->setGeometry(15,25,640-15-15,200);
    box->setStyleSheet(style_info);
    
    QLabel *shader_label = new QLabel(tr("Shader: "), this);
    shader_label->setGeometry(25, 250, 75, 30);
    shader_label->setStyleSheet(style_info);
    
    
    in_shader = new QLineEdit("acidcam.glsl", this);
    in_shader->setGeometry(115, 250, 250, 30);
    in_shader->setStyleSheet(style_info);
   
    QLabel *filter_label = new QLabel(tr("Filter: "), this);
    filter_label->setGeometry(25, 255+25, 75, 30);
    filter_label->setStyleSheet(style_info);
  
    in_filter = new QLineEdit("No Filter", this);
    in_filter->setGeometry(115, 255+25, 250, 30);
    in_filter->setStyleSheet(style_info);
    
    QLabel *frames_label = new QLabel(tr("Frames: "), this);
    frames_label->setGeometry(25, 255+25+30+5, 75, 30);
    frames_label->setStyleSheet(style_info);
  
    in_frames = new QLineEdit("10", this);
    in_frames->setGeometry(115, 255+25+30+5, 250, 30);
    in_frames->setStyleSheet(style_info);
    
    auto_add = new QPushButton(tr("Add"), this);
    auto_add->setGeometry(25, 255+25+30+5+35, 100, 30);
    auto_add->setStyleSheet(style_info);
    
    auto_remove = new QPushButton(tr("Remove"), this);
    auto_remove->setGeometry(25+105+5, 255+25+30+5+35, 100, 30);
    auto_remove->setStyleSheet(style_info);
    
    auto_insert = new QPushButton(tr("Insert"), this);
    auto_insert->setGeometry(25+105+105+10, 255+25+30+5+35, 100, 30);
    auto_insert->setStyleSheet(style_info);
    
    auto_save = new QPushButton(tr("Save"), this);
    auto_save->setGeometry(25, 255+25+30+35+35+5, 100, 30);
    auto_save->setStyleSheet(style_info);
    
    auto_load = new QPushButton(tr("Load"), this);
    auto_load->setGeometry(25+105+5, 255+25+30+5+35+30+5, 100, 30);
    auto_load->setStyleSheet(style_info);
    
    connect(auto_add, SIGNAL(clicked()), this, SLOT(btn_Add()));
    connect(auto_remove, SIGNAL(clicked()), this, SLOT(btn_Rmv()));
    connect(auto_insert, SIGNAL(clicked()), this, SLOT(btn_Insert()));
    connect(auto_save, SIGNAL(clicked()), this, SLOT(btn_Save()));
    connect(auto_load, SIGNAL(clicked()), this, SLOT(btn_Load()));
    
}


void Auto::btn_Add() {
    QString text;
    QTextStream stream(&text);
    stream << in_shader->text().trimmed() << ":" << in_filter->text().trimmed() << ":" << in_frames->text().trimmed();
    box->addItem(text.trimmed());
}

void Auto::btn_Rmv() {
    int pos = box->currentRow();
    if(pos != -1) {
        box->takeItem(pos);
    }
}

void Auto::btn_Insert() {
    int pos = box->currentRow();
    if(pos != -1) {
        QString text;
        QTextStream stream(&text);
        stream << in_shader->text().trimmed() << ":" << in_filter->text().trimmed() << ":" << in_frames->text().trimmed();
        QListWidgetItem *item = box->item(pos);
        item->setText(text);
    }
}

void Auto::btn_Save() {
    QString name = QFileDialog::getSaveFileName(this,tr("Save AutoFilter File"), "autofilter", tr("AF Files (*.af)"));

    if(name != "") {
        std::fstream file;
        file.open(name.toStdString(), std::ios::out);
        if(!file.is_open()) {
            return;
        }
        for(int i = 0; i < box->count(); ++i) {
            QListWidgetItem *item = box->item(i);
            file << item->text().toStdString() << "\n";
        }
        file.close();
    }
}

void Auto::btn_Load() {
   
    QString name = QFileDialog::getOpenFileName(this,tr("Open AutoFilter file"), "autofilter", tr("AF Files (*.af)"));
    
    if(name != "") {
        std::fstream file;
        file.open(name.toStdString(), std::ios::in);
        if(!file.is_open()) {
            return;
        }
        
        box->clear();
        
        while(!file.eof()) {
            std::string s;
            std::getline(file, s);
            if(file) {
                box->addItem(s.c_str());
            }
        }
        file.close();
    }
}
