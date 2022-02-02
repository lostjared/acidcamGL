#ifndef __OPTIONS_WIN_H__
#define __OPTIONS_WIN_H__


#include<QDialog>
#include<QLineEdit>
#include<QCheckBox>

class Options : public QDialog {
Q_OBJECT
public:
    Options(QWidget *parent = 0);
    
public slots:
    void chkClicked();
    void hideWindow();
    
private:
    void create_window();
};




#endif

