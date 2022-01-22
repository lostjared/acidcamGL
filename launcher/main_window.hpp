
#ifndef __MAIN_WINDOW_H_
#define __MAIN_WINDOW_H_

#include<QMainWindow>
#include<QTextEdit>
#include<QLineEdit>
#include<QPushButton>
#include<QThread>
#include<QLabel>
#include<QComboBox>
#include<QCheckBox>
#include<QProcess>
#include<vector>
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
    void selectShaders();
    void selectVideo();
    void selectPath();
    void setMatPath();
    void setPlaylistPath();
    void setAutoFilter();
signals:
    void LogString(const QString &text);
private:
    QTextEdit *command_stdout;
    QLineEdit *command;
    QPushButton *start_button;
    QComboBox *mode_select;
    QLineEdit *device_edit;
    QPushButton *select_video;
    QLineEdit *select_video_text;
    QPushButton *select_filters;
    QLineEdit *select_filters_text;
    QStringList cmd_list;
    QPushButton *select_path;
    QLineEdit *select_path_text;
    QLineEdit *camera_res;
    QLineEdit *window_res;
    QCheckBox *full_screen;
    QCheckBox *full_screen_resize;
    QLineEdit *monitor_;
    QCheckBox *syphon_enabled;
    QLineEdit *start_shader;
    QLineEdit *start_filter;
    QLineEdit *start_sec;
    QCheckBox *record_video;
    QComboBox *record_type;
    QLineEdit *record_name;
    QLineEdit *record_crf;
    QLineEdit *material_filename;
    QPushButton *material_set;
    QLineEdit *playlist_file;
    QPushButton *playlist_set;
    QCheckBox *enable_playback;
    QLineEdit *enable_bpm;
    QLineEdit *auto_filter;
    QPushButton *auto_set;
    std::vector<FILE*> fptr;
};


#endif

