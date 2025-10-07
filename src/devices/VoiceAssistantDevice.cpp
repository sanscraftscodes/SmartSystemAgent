#include "VoiceAssistantDevice.h"
#include <iostream>
#include <thread>
#include <cstdio>
#include <memory>
#include <array>
#include <sapi.h>
#include <atlbase.h>
#pragma comment(lib, "sapi.lib")

bool VoiceAssistantDevice::handles(const std::string &action) const {
    return action == "Speak" || action == "VoiceOn" || action == "VoiceOff";
}

void VoiceAssistantDevice::operate(const std::string &action, const std::string &args) {
    if (action == "Speak") {
        if (args.empty()) { std::cout << "[TTS] No text provided to Speak.\n"; return; }
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
        if (FAILED(hr)) { std::cout << "[TTS] COM init failed.\n"; return; }
        ISpVoice *pVoice = nullptr;
        hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
        if (SUCCEEDED(hr) && pVoice) {
            int size = MultiByteToWideChar(CP_UTF8, 0, args.c_str(), -1, NULL, 0);
            std::wstring wmsg(size, 0);
            MultiByteToWideChar(CP_UTF8, 0, args.c_str(), -1, &wmsg[0], size);
            pVoice->Speak(wmsg.c_str(), SPF_ASYNC, NULL);
            pVoice->Release();
            std::cout << "[TTS] Speaking: " << args << "\n";
        } else {
            std::cout << "[TTS] Failed to create voice.\n";
        }
        CoUninitialize();
    }
    else if (action == "VoiceOn") {
        if (voiceEnabled) { std::cout << "[Voice] Already enabled.\n"; return; }
        voiceEnabled = true;
        std::thread(&VoiceAssistantDevice::startListening, this).detach();
    }
    else if (action == "VoiceOff") {
        stopListening();
    }
}

void VoiceAssistantDevice::startListening() {
    std::cout << "[Voice] Listening mode activated.\n";
    while (voiceEnabled) {
        // Run Python script to capture one command
        std::array<char, 128> buffer;
        std::string result;
        FILE* pipe = _popen("python voice_input.py", "r");
        if (!pipe) {
            std::cout << "[Voice] Failed to run voice_input.py\n";
            break;
        }
        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
            result += buffer.data();
        _pclose(pipe);

        // Clean up result
        if (!result.empty()) {
            std::string trimmed;
            for (char c : result) {
                if (isprint(c) || c == ' ') trimmed += c;
            }
            if (!trimmed.empty()) {
                std::cout << "[Voice->Cmd] " << trimmed << "\n";
                if (trimmed == "exit" || trimmed == "quit") {
                    std::cout << "[Voice] Exit command detected. Stopping voice mode.\n";
                    stopListening();
                    break;
                }
                // You can later feed this to your CommandParser via C++ integration
                system(("SmartSysAgent.exe " + trimmed).c_str());
            }
        }
    }
    std::cout << "[Voice] Listening stopped.\n";
}

void VoiceAssistantDevice::stopListening() {
    voiceEnabled = false;
    std::cout << "[Voice] Voice mode disabled.\n";
}