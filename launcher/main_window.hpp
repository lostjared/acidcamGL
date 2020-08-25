
#ifndef __MAIN_WINDOW_H_
#define __MAIN_WINDOW_H_

#include<QMainWindow>
#include<QTextEdit>
#include<QLineEdit>
#include<QPushButton>

class MainWindow : public QMainWindow {

private:
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
   void Log(const QString &text);
public slots:
    void launchProgram();
private:
    QTextEdit *command_stdout;
    QLineEdit *command;
    QPushButton *start_button;
};



#endif

