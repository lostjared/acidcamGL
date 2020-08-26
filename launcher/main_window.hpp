
#ifndef __MAIN_WINDOW_H_
#define __MAIN_WINDOW_H_

#include<QMainWindow>
#include<QTextEdit>
#include<QLineEdit>
#include<QPushButton>
#include<QThread>
#include<QLabel>
#include<QComboBox>
#include"sock_server.hpp"



class MainWindow : public QMainWindow {

private:
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
public slots:
    void launchProgram();
    void LogMessage(const QString &text);
    void Log(const QString &text);
    void updateCommand();
    void textChanged_device(const QString &text);
    void comboChanged_mode(int index);
signals:
    void LogString(const QString &text);
private:
    QTextEdit *command_stdout;
    QLineEdit *command;
    QPushButton *start_button;
    QComboBox *mode_select;
    QLineEdit *device_edit;
};


#endif

