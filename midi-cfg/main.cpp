#include "midi_cfg.hpp"
#include <iostream>
#include "RtMidi.h"
#include <array>
#include<thread>
#include<chrono>

bool chooseMidiPort(RtMidiIn* rtmidi);
RtMidi::Api chooseMidiApi();
void midi_cleanup();
RtMidiIn* midiin = nullptr;
std::vector<unsigned char> bytes;
bool done = false;
int setup_main();

std::vector<std::array<std::string, 4>> knobs{
    {"GLFW_KEY_LEFT_RIGHT_Knob", "262", "263", "Filter index move left/right"},
    {"GLFW_KEY_UP_DOWN_Knob", "264", "265", "Shader Index move down/up"},
    {"TimeForward_Knob", "500", "501", "Move Time Forward/Back"},
};

std::vector<std::array<std::string, 3>> keys{
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
    {"GLFW_KEY_SLASH", "47", "Random Shader Toggle"}
};

template<typename T>
bool contains(const std::vector<T>& c, const std::vector<T>& sub) {
    if (sub.empty()) {
        return true;
    }
    if (c.size() < sub.size()) {
        return false;
    }
    auto it = std::search(c.begin(), c.end(), sub.begin(), sub.end());
    return it != c.end();
}

void clearMidiMessages() {
    std::vector<unsigned char> message;
    do {
        midiin->getMessage(&message);
    } while (!message.empty());
}

int main(int argc, char** argv) {
    std::string output_file = "midi.midi_cfg";

    if (argc == 2) {
        output_file = argv[1];
    }

    midi::MIDI_Config config;
    if (setup_main() != 0) {
        std::cerr << "Error could not open MIDI device...\n";
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < knobs.size(); ++i) {
        std::cout << "Press the Knob up or down\n";
        done = false;
        std::cout << knobs[i][0] << " value " << knobs[i][1] << ":" << knobs[i][2] << "\n";
        std::cout << "Description: " << knobs[i][3] << "\n";

        // Clear leftover messages
        clearMidiMessages();

        std::vector<unsigned char> message;
        while (message.empty()) {
            midiin->getMessage(&message);
        }
        std::cout << "MIDI Key - [ ";
        for (int q = 0; q < message.size(); ++q)
            std::cout << static_cast<int>(message[q]) << " ";
        std::cout << "]\n";
        int keycode = message[1];

        config.addCode({ atoi(knobs[i][1].c_str()), atoi(knobs[i][2].c_str()) }, midi::Key(message[0], message[1], message[2]));
        std::cout << "Keycode added\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
    }

    for (int i = 0; i < keys.size(); ++i) {
        std::cout << "Press Key on Controller for Keyboard Value: \n";
        done = false;
        std::cout << keys[i][0] << " value: " << keys[i][1] << "\n";
        std::cout << "Description: " << keys[i][2] << "\n";
        int num = 0;

        // Clear leftover messages
        clearMidiMessages();

        while (!done) {
            std::vector<unsigned char> message;
            midiin->getMessage(&message);
            if (!message.empty() && contains(bytes, message) == false) {
                bytes.insert(bytes.end(), message.begin(), message.end());
                std::cout << "MIDI Key - [ ";
                for (int q = 0; q < message.size(); ++q)
                    std::cout << static_cast<int>(message[q]) << " ";
                std::cout << "]\n";
                num++;
                if (num >= 2) {
                    done = true;
                }
            }
        }
        done = false;

        int ud = 0;

        do {
            std::cout << "Do you wish to use the Press Down, or Press Up:\n";
            std::cout << "1 - Key Down\n2 - Key Up\n3 - Skip and Write File\n4 - Skip and Continue\n";
            std::string input;
            std::getline(std::cin, input);
            if (input.length() > 0) {
                ud = atoi(input.c_str());
            }
        } while (ud != 1 && ud != 2 && ud != 3 && ud != 4);

        if (ud == 1 && bytes.size() >= 2)
            config.addCode({ atoi(keys[i][0].c_str()), 0 }, midi::Key(bytes[0], bytes[1], bytes[2]));
        else if (ud == 2 && bytes.size() >= 5)
            config.addCode({ atoi(keys[i][1].c_str()), 0 }, midi::Key(bytes[3], bytes[4], bytes[5]));
        else if (ud == 3)
            break;

        if (!bytes.empty())
            bytes.erase(bytes.begin(), bytes.end());

        if (ud == 4) {
            std::cout << "Keycode skipped.\n";
            continue;
        }
        std::cout << "Keycode added\n";
        std::this_thread::sleep_for(std::chrono::seconds(3));
        // Clear leftover messages
        clearMidiMessages();
    }

    midi_cleanup();
    config.write(output_file);
    std::cout << "Wrote: " << output_file << "\n";
    return 0;
}

int setup_main() {
    try {
        midiin = new RtMidiIn(chooseMidiApi());
        if (chooseMidiPort(midiin) == false) {
            midi_cleanup();
            return 1;
        }
        midiin->ignoreTypes(false, false, false);
    }
    catch (RtMidiError& error) {
        error.printMessage();
    }
    return 0;
}

void midi_cleanup() {
    if (midiin != nullptr)
        delete midiin;
}

bool chooseMidiPort(RtMidiIn* rtmidi) {
    std::string portName;
    unsigned int i = 0, nPorts = rtmidi->getPortCount();
    if (nPorts == 0) {
        std::cout << "No input ports available!" << std::endl;
        return false;
    }

    if (nPorts == 1) {
        std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
        rtmidi->openPort(0); // Open the only available port
    }
    else {
        for (i = 0; i < nPorts; i++) {
            portName = rtmidi->getPortName(i);
            std::cout << "  Input port #" << i << ": " << portName << '\n';
        }

        do {
            std::cout << "\nChoose a port number: ";
            std::cin >> i;
        } while (i >= nPorts);
        std::string keyHit;
        std::getline(std::cin, keyHit); // used to clear out stdin
        rtmidi->openPort(i); // Open the chosen port
    }

    return true;
}

RtMidi::Api chooseMidiApi() {
    std::vector<RtMidi::Api> apis;
    RtMidi::getCompiledApi(apis);

    if (apis.size() <= 1)
        return RtMidi::Api::UNSPECIFIED;

    std::cout << "\nAPIs\n  API #0: unspecified / default\n";
    for (size_t n = 0; n < apis.size(); n++)
        std::cout << "  API #" << apis[n] << ": " << RtMidi::getApiDisplayName(apis[n]) << "\n";

    std::cout << "\nChoose an API number: ";
    unsigned int i;
    std::cin >> i;

    std::string dummy;
    std::getline(std::cin, dummy); // used to clear out stdin

    return static_cast<RtMidi::Api>(i);
}
