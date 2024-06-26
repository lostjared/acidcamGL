#ifndef KEYCAPTUREDIALOG_H
#define KEYCAPTUREDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QPushButton>
#include <vector>
#include <array>
#include "RtMidi.h"

class KeyCaptureDialog : public QDialog {
    Q_OBJECT

public:
    KeyCaptureDialog(RtMidiIn *midiin, const QString& keyDescription, QWidget *parent = nullptr);
    std::vector<unsigned char> getSelectedKey() const;
    bool isStopped() const;

private slots:
    void stopKeySelection();
    void captureKey(const std::vector<unsigned char> &message);

private:
    QVBoxLayout *layout;
    QLabel *instructionLabel;
    QListWidget *keyListWidget;
    QPushButton *okButton;
    QPushButton *stopButton;
    RtMidiIn *midiin;
    bool stopped;
    bool callbackSet;
    std::vector<std::vector<unsigned char>> capturedKeys;

    static void midiCallback(double deltatime, std::vector<unsigned char> *message, void *userData);
};

#endif