
#include"main_window.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
     setFixedSize(1280, 1280);
    setWindowTitle("acidcamGL - Start New Session");
    command_stdout = new QTextEdit("", this);
    command_stdout->setGeometry(5, 1280/2, 1280-10, 1280/2-5);
    command = new QLineEdit("", this);
    command->setGeometry(5, 10, 1280-10, 30);
    command_stdout->setReadOnly(true);
    command->setText("acidcamGL ");
 }
