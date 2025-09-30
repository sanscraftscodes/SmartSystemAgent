#include "MusicPlayerDevice.h"
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <filesystem>
#pragma comment(lib, "winmm.lib")

bool MusicPlayerDevice::handles(const std::string &action) const {
    return action == "PlayMusic" || action == "StopMusic";
}

void MusicPlayerDevice::operate(const std::string &action, const std::string &args) {
    if (action == "PlayMusic") {
        if (args.empty()) { std::cout << "[Music] Provide path to an audio file.\n"; return; }
        // Prefer WAV via PlaySound, otherwise launch default player for file
        std::filesystem::path p(args);
        if (p.extension() == ".wav" || p.extension() == ".WAV") {
            PlaySoundA(args.c_str(), NULL, SND_FILENAME | SND_ASYNC);
            std::cout << "[Music] Playing WAV: " << args << "\n";
        } else {
            // launch default associated app
            std::wstring wpath = std::filesystem::path(args).wstring();
            HINSTANCE res = ShellExecuteW(NULL, L"open", wpath.c_str(), NULL, NULL, SW_SHOWNORMAL);
            if ((intptr_t)res <= 32) {
                std::cout << "[Music] Failed to open music file via default app.\n";
            } else {
                std::cout << "[Music] Launched default player for: " << args << "\n";
            }
        }
    } else {
        PlaySound(NULL, 0, 0);
        std::cout << "[Music] Stopped (if using PlaySound).\n";
    }
}
