#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include "RtMidi.h"
#include "midi_cfg.hpp"
#include <vector>
#include <array>
#include <QThread>
#include"cap.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void knobInputProcessed();
    void keyInputProcessed();
    void startKnobInputProcessing(int index);

private slots:
    void setupMidi();
    void chooseMidiPort();
    void handleKnobPress();
    void handleKeyPress();
    void saveConfig();
    void onKnobInputReceived(std::vector<unsigned char> message, int index);
    void onKeyInputReceived(std::vector<unsigned char> downMessage, std::vector<unsigned char> upMessage);

private:
    QVBoxLayout *layout;
    QLabel *statusLabel;
    QPushButton *setupButton;
    QPushButton *choosePortButton;
    QPushButton *knobButton;
    QPushButton *keyButton;
    QPushButton *saveButton;
    QLineEdit *outputFileLineEdit;

    RtMidiIn *midiin;
    bool done;
    std::vector<unsigned char> downBytes;
    std::vector<unsigned char> upBytes;
    std::vector<std::array<std::string, 4>> knobs;
    std::vector<std::array<std::string, 3>> keys;

    void clearMidiMessages();
    int promptUserForKeyChoice(midi::MIDI_Config &config, int keyIndex);

    midi::MIDI_Config config;
};

#endif 