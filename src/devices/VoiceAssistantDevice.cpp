// #include "VoiceAssistantDevice.h"
// #include <sapi.h>
// #include <atlbase.h>
// #include <iostream>
// #pragma comment(lib, "sapi.lib")

// bool VoiceAssistantDevice::handles(const std::string &action) const {
//     return action == "Speak";
// }

// void VoiceAssistantDevice::operate(const std::string &action, const std::string &args) {
//     if (args.empty()) { std::cout << "[TTS] No text provided to Speak.\n"; return; }
//     HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
//     if (FAILED(hr)) { std::cout << "[TTS] COM init failed.\n"; return; }
//     ISpVoice *pVoice = nullptr;
//     hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void**)&pVoice);
//     if (SUCCEEDED(hr) && pVoice) {
//         int size = MultiByteToWideChar(CP_UTF8, 0, args.c_str(), -1, NULL, 0);
//         std::wstring wmsg(size, 0);
//         MultiByteToWideChar(CP_UTF8, 0, args.c_str(), -1, &wmsg[0], size);
//         pVoice->Speak(wmsg.c_str(), SPF_ASYNC, NULL); // asynchronous
//         pVoice->Release();
//         std::cout << "[TTS] Speaking: " << args << "\n";
//     } else {
//         std::cout << "[TTS] Failed to create voice.\n";
//     }
//     CoUninitialize();
// }
