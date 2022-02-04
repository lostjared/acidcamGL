#ifndef __OPTIONS_WIN_H__
#define __OPTIONS_WIN_H__


#include<QDialog>
#include<QLineEdit>
#include<QCheckBox>
#include<QPushButton>
#include<QSettings>
#include<QFileDialog>

class Options : public QDialog {
Q_OBJECT
public:
    Options(QWidget *parent = 0);
    ~Options();
    void load();
    void save();

public slots:
    void chkClicked();
    void selectExecutable();
    void okClicked();
    void setDefaults();
public:
    QLineEdit *exec_path;
    QCheckBox *exec_enable;
    QPushButton *exec_select;
    QSettings *settings;
    QPushButton *ok_button, *def_button;
private:
    void create_window();
};




#endif

