#ifndef _AUTO_WINDOW_H__
#define _AUTO_WINDOW_H__

#include<QDialog>
#include<QListWidget>
#include<QLineEdit>
#include<QLabel>
#include<QPushButton>

class Auto : public QDialog {
Q_OBJECT
    
public:
    Auto(QWidget *parent = 0);
    void createControls();
    
public slots:
    void btn_Add();
    void btn_Rmv();
    void btn_Insert();
    void btn_Save();
    void btn_Load();
    
private:
    QListWidget *box;
    QLineEdit *in_filter, *in_shader, *in_frames;
    QPushButton *auto_add, *auto_remove, *auto_insert, *auto_save, *auto_load;
};






#endif

