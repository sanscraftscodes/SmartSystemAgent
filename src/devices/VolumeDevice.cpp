#include "VolumeDevice.h"
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <iostream>
#pragma comment(lib, "ole32.lib")

bool VolumeDevice::handles(const std::string &action) const {
    return action == "SetVolume" || action == "GetVolume";
}

static bool setSystemVolumeScalar(float level) {
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) return false;
    IMMDeviceEnumerator *pEnum = nullptr;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
                          __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
    if (FAILED(hr) || !pEnum) { CoUninitialize(); return false; }
    IMMDevice *pDevice = nullptr;
    hr = pEnum->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr) || !pDevice) { pEnum->Release(); CoUninitialize(); return false; }
    IAudioEndpointVolume *pVolume = nullptr;
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pVolume);
    if (FAILED(hr) || !pVolume) { pDevice->Release(); pEnum->Release(); CoUninitialize(); return false; }
    hr = pVolume->SetMasterVolumeLevelScalar(level, NULL);
    pVolume->Release(); pDevice->Release(); pEnum->Release(); CoUninitialize();
    return SUCCEEDED(hr);
}

static bool getSystemVolumeScalar(float &outLevel) {
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) return false;
    IMMDeviceEnumerator *pEnum = nullptr;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL,
                          __uuidof(IMMDeviceEnumerator), (void**)&pEnum);
    if (FAILED(hr) || !pEnum) { CoUninitialize(); return false; }
    IMMDevice *pDevice = nullptr;
    hr = pEnum->GetDefaultAudioEndpoint(eRender, eConsole, &pDevice);
    if (FAILED(hr) || !pDevice) { pEnum->Release(); CoUninitialize(); return false; }
    IAudioEndpointVolume *pVolume = nullptr;
    hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pVolume);
    if (FAILED(hr) || !pVolume) { pDevice->Release(); pEnum->Release(); CoUninitialize(); return false; }
    float level = 0.0f;
    hr = pVolume->GetMasterVolumeLevelScalar(&level);
    if (SUCCEEDED(hr)) outLevel = level;
    pVolume->Release(); pDevice->Release(); pEnum->Release(); CoUninitialize();
    return SUCCEEDED(hr);
}

void VolumeDevice::operate(const std::string &action, const std::string &args) {
    if (action == "SetVolume") {
        if (args.empty()) { std::cout<<"[Volume] Provide volume percent (0-100).\n"; return; }
        try {
            int v = std::stoi(args);
            if (v<0) v=0; if (v>100) v=100;
            float scalar = v / 100.0f;
            if (setSystemVolumeScalar(scalar)) std::cout<<"[Volume] Set to "<<v<<"%\n";
            else std::cout<<"[Volume] Failed to set volume.\n";
        } catch(...) { std::cout<<"[Volume] Invalid argument.\n"; }
    } else if (action == "GetVolume") {
        float level=0.0f;
        if (getSystemVolumeScalar(level)) std::cout<<"[Volume] "<<int(level*100)<<"%\n";
        else std::cout<<"[Volume] Failed to read volume.\n";
    }
}
