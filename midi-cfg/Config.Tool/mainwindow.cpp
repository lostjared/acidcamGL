#include "mainwindow.h"
#include "cap.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), midiin(nullptr), done(false) {
    QWidget *centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);

    statusLabel = new QLabel("Status: Not connected", this);
    setupButton = new QPushButton("Setup MIDI", this);
    choosePortButton = new QPushButton("Choose MIDI Port", this);
    knobButton = new QPushButton("Configure Knobs", this);
    keyButton = new QPushButton("Configure Keys", this);
    saveButton = new QPushButton("Save Config", this);
    outputFileLineEdit = new QLineEdit("midi.midi_cfg", this);

    layout->addWidget(statusLabel);
    layout->addWidget(setupButton);
    layout->addWidget(choosePortButton);
    layout->addWidget(knobButton);
    layout->addWidget(keyButton);
    layout->addWidget(new QLabel("Output File:", this));
    layout->addWidget(outputFileLineEdit);
    layout->addWidget(saveButton);

    setCentralWidget(centralWidget);

    connect(setupButton, &QPushButton::clicked, this, &MainWindow::setupMidi);
    connect(choosePortButton, &QPushButton::clicked, this, &MainWindow::chooseMidiPort);
    connect(knobButton, &QPushButton::clicked, this, &MainWindow::handleKnobPress);
    connect(keyButton, &QPushButton::clicked, this, &MainWindow::handleKeyPress);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveConfig);

    knobButton->setEnabled(false);
    keyButton->setEnabled(false);

    // Initialize knobs and keys
    knobs = {
        {"GLFW_KEY_LEFT_RIGHT_Knob", "262", "263", "Filter index move left/right"},
        {"GLFW_KEY_UP_DOWN_Knob", "264", "265", "Shader Index move down/up"},
        {"TimeForward_Knob", "500", "501", "Move Time Forward/Back"},
    };

    keys = {
        {"GLFW_KEY_LEFT", "263", "Filter index move left"},
        {"GLFW_KEY_RIGHT", "262", "Filter index move right"},
        {"GLFW_KEY_UP", "265", "Shader Index move up"},
        {"GLFW_KEY_DOWN", "264", "Shader Index move down"},
        {"GLFW_KEY_SPACE", "32", "Acid Cam filters enabled/disabled"},
        {"TimeForward", "500", "Move Time Forward"},
        {"TimeBackward", "501", "Move Time Backward"},
        {"TimePause", "502", "Pause Time"},
        {"TimeOnOff", "503", "Toggle On/Off"},
        {"GLFW_KEY_EQUAL", "61", "increase blend percentage"},
        {"GLFW_KEY_MINUS", "45", "decrease blend percentage"},
        {"GLFW_KEY_H", "72", "Shuffle Playlist"},
        {"GLFW_KEY_L", "76", "Enable disable playlist"},
        {"GLFW_KEY_N", "78", "set index to the end"},
        {"GLFW_KEY_P", "80", "index reset to zero"},
        {"GLFW_KEY_PAGE_DOWN", "267", "Restore Position Index"},
        {"GLFW_KEY_PAGE_UP", "266", "Store Index Position"},
        {"GLFW_KEY_COMMA", "44", "Color Map Decrease"},
        {"GLFW_KEY_PERIOD", "46", "Color Map Increase"},
        {"GLFW_KEY_SLASH", "47", "Random Shader Toggle"},
    };

    setGeometry(100, 100, 400, 300);
}

MainWindow::~MainWindow() {
    if (midiin != nullptr) {
        delete midiin;
    }
}

void MainWindow::setupMidi() {
    try {
        midiin = new RtMidiIn();
        midiin->ignoreTypes(false, false, false);
        statusLabel->setText("Status: MIDI setup complete");
        qDebug() << "MIDI setup complete";
    } catch (RtMidiError &error) {
        QMessageBox::critical(this, "Error", "Could not initialize MIDI device.");
        qDebug() << "Error initializing MIDI device:" << error.getMessage().c_str();
    }
}

void MainWindow::chooseMidiPort() {
    if (midiin == nullptr) {
        QMessageBox::warning(this, "Warning", "MIDI device not set up.");
        return;
    }

    unsigned int nPorts = midiin->getPortCount();
    if (nPorts == 0) {
        QMessageBox::information(this, "Information", "No input ports available!");
        qDebug() << "No input ports available!";
        return;
    }

    QStringList ports;
    for (unsigned int i = 0; i < nPorts; i++) {
        ports << QString::fromStdString(midiin->getPortName(i));
    }

    bool ok;
    QString port = QInputDialog::getItem(this, "Choose MIDI Port", "Port:", ports, 0, false, &ok);
    if (ok && !port.isEmpty()) {
        int index = ports.indexOf(port);
        midiin->openPort(index);
        statusLabel->setText("Status: MIDI port chosen");
        knobButton->setEnabled(true);
        keyButton->setEnabled(true);
        qDebug() << "MIDI port chosen:" << port;
    }
}

void MainWindow::handleKnobPress() {
    if (midiin == nullptr) {
        QMessageBox::warning(this, "Warning", "MIDI device not set up.");
        return;
    }

    for (size_t i = 0; i < knobs.size(); ++i) {
        KeyCaptureDialog dialog(midiin, QString::fromStdString(knobs[i][3]), this);
        qDebug() << "Opening KeyCaptureDialog for knob configuration:" << QString::fromStdString(knobs[i][3]);
        if (dialog.exec() == QDialog::Accepted) {
            if (dialog.isStopped()) {
                QMessageBox::information(this, "Stopped", "Knob configuration stopped.");
                qDebug() << "Knob configuration stopped";
                return;
            }
            std::vector<unsigned char> selectedKey = dialog.getSelectedKey();
            if (!selectedKey.empty()) {
                config.addCode({ std::stoi(knobs[i][1]), std::stoi(knobs[i][2]) }, midi::Key(selectedKey[0], selectedKey[1], selectedKey[2]));
                QMessageBox::information(this, "Knob set", "Knob Set");
                qDebug() << "Knob set with key:" << selectedKey[0] << selectedKey[1] << selectedKey[2];
            } else {
                QMessageBox::warning(this, "Warning", "No key selected. Please try again.");
                qDebug() << "No key selected";
            }
        } else if (dialog.isStopped()) {
            QMessageBox::information(this, "Stopped", "Knob configuration stopped.");
            qDebug() << "Knob configuration stopped";
            return;
        }
    }
}

void MainWindow::handleKeyPress() {
    if (midiin == nullptr) {
        QMessageBox::warning(this, "Warning", "MIDI device not set up.");
        return;
    }

    qDebug() << "handleKeyPress called";
    for (size_t i = 0; i < keys.size(); ++i) {
        KeyCaptureDialog dialog(midiin, QString::fromStdString(keys[i][2]), this);
        qDebug() << "Opening KeyCaptureDialog for key configuration:" << QString::fromStdString(keys[i][2]);
        if (dialog.exec() == QDialog::Accepted) {
            if (dialog.isStopped()) {
                QMessageBox::information(this, "Stopped", "Key configuration stopped.");
                qDebug() << "Key configuration stopped";
                return;
            }
            std::vector<unsigned char> selectedKey = dialog.getSelectedKey();
            if (!selectedKey.empty()) {
                config.addCode({ std::stoi(keys[i][1]), 0 }, midi::Key(selectedKey[0], selectedKey[1], selectedKey[2]));
                QMessageBox::information(this, "Key set", "Key Set");
                qDebug() << "Key set with key:" << selectedKey[0] << selectedKey[1] << selectedKey[2];
            } else {
                QMessageBox::warning(this, "Warning", "No key selected. Please try again.");
                qDebug() << "No key selected";
            }
        } else if (dialog.isStopped()) {
            QMessageBox::information(this, "Stopped", "Key configuration stopped.");
            qDebug() << "Key configuration stopped";
            return;
        }
    }
}

void MainWindow::saveConfig() {
#ifndef _WIN32
    QString fileName = QFileDialog::getSaveFileName(this, "Save Config File", outputFileLineEdit->text(), "Config Files (*.midi_cfg);;All Files (*)");
    if (fileName.isEmpty()) {
        qDebug() << "Save config canceled";
        return;
    }
    config.write(fileName.toStdString());
    QMessageBox::information(this, "Config Saved", "Configuration saved to " + fileName);
    qDebug() << "Configuration saved to:" << fileName;
#else
    config.write(outputFileLineEdit->text().toStdString());
    QMessageBox::information(this, "Config Saved", "Configuration saved to " + fileName);
#endif

}

void MainWindow::onKnobInputReceived(std::vector<unsigned char> message, int index) {
    qDebug() << "Knob input received:" << message[0] << message[1] << message[2] << "Index:" << index;
}

void MainWindow::onKeyInputReceived(std::vector<unsigned char> downMessage, std::vector<unsigned char> upMessage) {
    qDebug() << "Key down input received:" << downMessage[0] << downMessage[1] << downMessage[2];
    qDebug() << "Key up input received:" << upMessage[0] << upMessage[1] << upMessage[2];
}
