#include"options_window.hpp"


Options::Options(QWidget *parent) : QDialog(parent) {
    setFixedSize(600, 120);
    setWindowTitle("acidcamGL - Options");
    setWindowIcon(QPixmap(":/images/icon.png"));
    create_window();
}

void Options::chkClicked() {

    if(exec_enable->isChecked()) {
        exec_path->setEnabled(true);
        exec_select->setEnabled(true);
    }
    else {
        exec_path->setEnabled(false);
        exec_select->setEnabled(false);
    }
}

void Options::okClicked() {
    hide();
}
    
void Options::selectExecutable() {
    QString dir_path = settings->value("dpath1", "").toString();
    QString name = QFileDialog::getOpenFileName(this,     tr("Open EXE"), dir_path, tr("All Files (*.*)"));
    if(name.length() > 0) {
        exec_path->setText(name);
        std::string val = name.toStdString();
        auto pos = val.rfind("/");
        if(pos == std::string::npos)
            pos = val.rfind("\\");
        if(pos != std::string::npos) {
            val = val.substr(0, pos);
        }
        settings->setValue("dpath1", val.c_str());
    }
}
    
void Options::create_window() {
    QString style_info = "font-size: 16px; font-family: monaco;";
    exec_enable = new QCheckBox(tr("Executable Path:"), this);
    exec_enable->setGeometry(25, 25, 200, 30);
    exec_enable->setStyleSheet(style_info);
    
    connect(exec_enable, SIGNAL(clicked()), this, SLOT(chkClicked()));
    
    exec_path = new QLineEdit("acidcamGL", this);
    exec_path->setGeometry(210, 25, 250, 30);
    exec_path->setStyleSheet(style_info);
    exec_path->setEnabled(false);
    
    exec_select = new QPushButton(tr("Select"), this);
    exec_select->setGeometry(470, 25, 100, 30);
    exec_select->setStyleSheet(style_info);
    exec_select->setEnabled(false);
    
    connect(exec_select, SIGNAL(clicked()), this, SLOT(selectExecutable()));
    
    ok_button = new QPushButton(tr("Ok"), this);
    ok_button->setGeometry(470, 85, 100, 30);
    ok_button->setStyleSheet(style_info);
    
    connect(ok_button, SIGNAL(clicked()), this, SLOT(okClicked()));
    
}
