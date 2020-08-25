
#ifndef __MAIN_WINDOW_H_
#define __MAIN_WINDOW_H_

#include<QMainWindow>
#include<QTextEdit>
#include<QLineEdit>

class MainWindow : public QMainWindow {

private:
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    
private:
    QTextEdit *command_stdout;
    QLineEdit *command;
};



#endif

