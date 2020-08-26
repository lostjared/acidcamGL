
#ifndef __MAIN_WINDOW_H_
#define __MAIN_WINDOW_H_

#include<QMainWindow>
#include<QTextEdit>
#include<QLineEdit>
#include<QPushButton>
#include<QThread>
#include<QLabel>
#include<QComboBox>

class ServerThread : public QThread {
    Q_OBJECT
public:
    ServerThread();
public slots:
    void process();
signals:
    void Log(const QString &text);
};

class MainWindow : public QMainWindow {

private:
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
public slots:
    void launchProgram();
    void LogMessage(const QString &text);
    void Log(const QString &text);
signals:
    void LogString(const QString &text);
private:
    QTextEdit *command_stdout;
    QLineEdit *command;
    QPushButton *start_button;
    QComboBox *mode_select;
};


#endif

