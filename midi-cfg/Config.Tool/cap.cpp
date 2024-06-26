#include "cap.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>

bool KeyCaptureDialog::callbackSet = false; // Initialize static member

KeyCaptureDialog::KeyCaptureDialog(RtMidiIn *midiin, const QString& keyDescription, QWidget *parent)
    : QDialog(parent), midiin(midiin), stopped(false) {
    layout = new QVBoxLayout(this);

    instructionLabel = new QLabel("<b>" + keyDescription + "</b>", this);
    instructionLabel->setTextFormat(Qt::RichText);
    keyListWidget = new QListWidget(this);
    okButton = new QPushButton("OK", this);
    stopButton = new QPushButton("Stop", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(stopButton);

    layout->addWidget(instructionLabel);
    layout->addWidget(keyListWidget);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(stopButton, &QPushButton::clicked, this, &KeyCaptureDialog::stopKeySelection);

    // Set up RtMidi callback, ensuring no other callback is set
    if (!callbackSet) {
        midiin->setCallback(&KeyCaptureDialog::midiCallback, this);
        midiin->ignoreTypes(false, false, false);
        callbackSet = true;
    }

    qDebug() << "KeyCaptureDialog initialized with description:" << keyDescription;
}

std::vector<unsigned char> KeyCaptureDialog::getSelectedKey() const {
    QListWidgetItem *selectedItem = keyListWidget->currentItem();
    if (selectedItem) {
        return capturedKeys[keyListWidget->row(selectedItem)];
    }
    return {};
}

bool KeyCaptureDialog::isStopped() const {
    return stopped;
}

void KeyCaptureDialog::captureKey(const std::vector<unsigned char> &message) {
    qDebug() << "Key captured:" << message[0] << message[1] << message[2];
    capturedKeys.push_back(message);
    QString keyStr = QString("Key: %1 %2 %3")
        .arg(static_cast<int>(message[0]))
        .arg(static_cast<int>(message[1]))
        .arg(static_cast<int>(message[2]));
    keyListWidget->addItem(keyStr);
}

void KeyCaptureDialog::stopKeySelection() {
    qDebug() << "Key selection stopped";
    stopped = true;
    reject();
}

void KeyCaptureDialog::midiCallback(double, std::vector<unsigned char> *message, void *userData) {
    if (message->empty()) return;
    KeyCaptureDialog *dialog = static_cast<KeyCaptureDialog*>(userData);
    dialog->captureKey(*message);
}