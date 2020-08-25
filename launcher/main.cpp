#include"main_window.hpp"
#include<QApplication>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    MainWindow window1;
    window1.show();
    return app.exec();
}
