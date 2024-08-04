#include <iostream>
#include <rtaudio/RtAudio.h>

int main() {
    RtAudio audio;
    unsigned int deviceCount = audio.getDeviceCount();
    if (deviceCount == 0) {
        std::cout << "No audio devices found!" << std::endl;
        return 1;
    }

    std::cout << "Number of audio devices: " << deviceCount << std::endl;
    for (unsigned int i = 0; i < deviceCount; i++) {
        RtAudio::DeviceInfo info = audio.getDeviceInfo(i);
        std::cout << "--------------------------------" << std::endl;
        std::cout << "Device ID: " << i << std::endl;
        std::cout << "Name: " << info.name << std::endl;
        std::cout << "Output Channels: " << info.outputChannels << std::endl;
        std::cout << "Input Channels: " << info.inputChannels << std::endl;
        std::cout << "Duplex Channels: " << info.duplexChannels << std::endl;
        std::cout << "Sample Rates: ";
        for (size_t j = 0; j < info.sampleRates.size(); j++) {
            std::cout << info.sampleRates[j] << " ";
        }
        std::cout << std::endl;
        std::cout << "Preferred Sample Rate: " << info.preferredSampleRate << std::endl;
        std::cout << "Is Default Output: " << (info.isDefaultOutput ? "Yes" : "No") << std::endl;
        std::cout << "Is Default Input: " << (info.isDefaultInput ? "Yes" : "No") << std::endl;
    }

    return 0;
}