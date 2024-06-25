#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QThread>
#include <QTimer>
#include <thread>
#include <chrono>
#include <QDebug>
#include<QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), midiin(nullptr), done(false) {
    // Initialize the layout and widgets
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

    // Connect signals and slots
    connect(setupButton, &QPushButton::clicked, this, &MainWindow::setupMidi);
    connect(choosePortButton, &QPushButton::clicked, this, &MainWindow::chooseMidiPort);
    connect(knobButton, &QPushButton::clicked, this, &MainWindow::handleKnobPress);
    connect(keyButton, &QPushButton::clicked, this, &MainWindow::handleKeyPress);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveConfig);

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
    } catch (RtMidiError &error) {
        QMessageBox::critical(this, "Error", "Could not initialize MIDI device.");
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
    }
}

void MainWindow::clearMidiMessages() {
    std::vector<unsigned char> message;
    do {
        midiin->getMessage(&message);
    } while (!message.empty());
}

void MainWindow::handleKnobPress() {
    if (midiin == nullptr) {
        QMessageBox::warning(this, "Warning", "MIDI device not set up.");
        return;
    }

   for (size_t i = 0; i < knobs.size(); ++i) {
        QMessageBox::information(this, "Knob Configuration", QString::fromStdString(knobs[i][3]) + "\nTurn Knob Now then press Ok");
        while(downBytes.empty()) {
            midiin->getMessage(&downBytes);
        }
        config.addCode({ std::stoi(knobs[i][1]), std::stoi(knobs[i][2]) }, midi::Key(downBytes[0], downBytes[1], downBytes[2]));
        done = false;
        
        QMessageBox::information(this, "Knob set", "Knob Set");
        std::this_thread::sleep_for(std::chrono::seconds(3));
        clearMidiMessages();
        downBytes.clear();
    }
}

void MainWindow::handleKeyPress() {
    if (midiin == nullptr) {
        QMessageBox::warning(this, "Warning", "MIDI device not set up.");
        return;
    }


    for (size_t i = 0; i < keys.size(); ++i) {
        QMessageBox::information(this, "Key Configuration", QString::fromStdString(keys[i][2]) + "\nPress Key Now then click Ok");

        downBytes.clear();
        upBytes.clear();
        done = false;

        while(downBytes.empty()) {
            midiin->getMessage(&downBytes);
        }
        while(upBytes.empty()) {
            midiin->getMessage(&upBytes);
        }
        if (!downBytes.empty() && !upBytes.empty()) {
          if(promptUserForKeyChoice(config, i) == 1) {
            done = true;
            return;
          }
          done = true;
        }
        if (!done) {
            QMessageBox::warning(this, "Warning", "Failed to capture both key down and key up events. Please try again.");
        }
    }
}

int MainWindow::promptUserForKeyChoice(midi::MIDI_Config &config, int keyIndex) {
    QMessageBox::information(this, "MIDI Key Pressed", QString("MIDI Key Down - [ %1 %2 %3 ]\nMIDI Key Up - [ %4 %5 %6 ]")
                             .arg(static_cast<int>(downBytes[0])).arg(static_cast<int>(downBytes[1])).arg(static_cast<int>(downBytes[2]))
                             .arg(static_cast<int>(upBytes[0])).arg(static_cast<int>(upBytes[1])).arg(static_cast<int>(upBytes[2])));

    int ud = 0;
    do {
        QString input = QInputDialog::getText(this, "Key Configuration", "Do you wish to use the Press Down, or Press Up:\n1 - Key Down\n2 - Key Up\n3 - Skip and Write File\n4 - Skip and Continue");
        ud = input.toInt();
    } while (ud != 1 && ud != 2 && ud != 3 && ud != 4);

    if (ud == 1 && downBytes.size() >= 3)
        config.addCode({ std::stoi(keys[keyIndex][1]), 0 }, midi::Key(downBytes[0], downBytes[1], downBytes[2]));
    else if (ud == 2 && upBytes.size() >= 3)
        config.addCode({ std::stoi(keys[keyIndex][1]), 0 }, midi::Key(upBytes[0], upBytes[1], upBytes[2]));
    else if (ud == 3) {
        saveConfig();
        return 1;
    }

    if (!downBytes.empty())
        downBytes.clear();
    if (!upBytes.empty())
        upBytes.clear();

    if (ud == 4) {
        QMessageBox::information(this, "Key Configuration", "Keycode skipped.");
        return 0;
    }
    QMessageBox::information(this, "Key Configuration", "Keycode added");
    std::this_thread::sleep_for(std::chrono::seconds(3));
    clearMidiMessages();
    return 0;
}

void MainWindow::saveConfig() {
    config.write(outputFileLineEdit->text().toStdString());
    QMessageBox::information(this, "Wrote to current Directory", "Worte Config to Current Directory");
}

void MainWindow::onMidiInputCaptured(std::vector<unsigned char> message) {
    qDebug() << "MIDI message captured: " << QByteArray::fromRawData(reinterpret_cast<const char*>(message.data()), static_cast<int>(message.size())).toHex();
    
    if ((message[0] & 0xF0) == 0x80) {  // Note Off event
        upBytes = message;
        done = true;
    } else if ((message[0] & 0xF0) == 0x90) {  // Note On event
        downBytes = message;
    }
}
