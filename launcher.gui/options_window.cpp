#include"options_window.hpp"


Options::Options(QWidget *parent) : QDialog(parent) {
    setFixedSize(600, 120);
    setWindowTitle("acidcamGL - Options");
    setWindowIcon(QPixmap(":/images/icon.png"));
    create_window();
}

void Options::load() {
    QString prog_path = settings->value("prog_path", "acidcamGL").toString();
    exec_path->setText(prog_path);
    QString e_checked = settings->value("prog_chk", "0").toString();
    if(e_checked == "0") {
        exec_enable->setChecked(false);
    } else {
        exec_select->setEnabled(true);
        exec_path->setEnabled(true);
        exec_enable->setChecked(true);
    }
}

void Options::save() {
    settings->setValue("prog_path", exec_path->text());
    settings->setValue("prog_chk", exec_enable->isChecked() ? "1" : "0");
}

void Options::chkClicked() {
    if(exec_enable->isChecked()) {
        exec_path->setEnabled(true);
        exec_select->setEnabled(true);
        save();
    }
    else {
        exec_path->setEnabled(false);
        exec_select->setEnabled(false);
    }
}

void Options::okClicked() {
    save();
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
    ok_button->setGeometry(470, 60, 100, 30);
    ok_button->setStyleSheet(style_info);
    
    connect(ok_button, SIGNAL(clicked()), this, SLOT(okClicked()));
    
}
