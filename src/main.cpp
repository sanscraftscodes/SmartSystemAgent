#include <iostream>
#include <string>
#include <memory>
#include "../include/Device.h"
#include "../include/DeviceManager.h"
#include "../include/CommandParser.h"

// devices
#include "devices/BatteryDevice.h"
#include "devices/BrightnessDevice.h"
#include "devices/VolumeDevice.h"
#include "devices/MusicPlayerDevice.h"
#include "devices/VoiceAssistantDevice.h"
#include "devices/AppLauncherDevice.h"
#include "devices/WifiDevice.h"
#include "devices/BluetoothDevice.h"

int main() {
    DeviceManager dm;
    dm.registerDevice(std::make_unique<BatteryDevice>());
    dm.registerDevice(std::make_unique<BrightnessDevice>());
    dm.registerDevice(std::make_unique<VolumeDevice>());
    dm.registerDevice(std::make_unique<MusicPlayerDevice>());
    dm.registerDevice(std::make_unique<VoiceAssistantDevice>());
    dm.registerDevice(std::make_unique<AppLauncherDevice>());
    dm.registerDevice(std::make_unique<WifiDevice>());
    dm.registerDevice(std::make_unique<BluetoothDevice>());
    

    std::cout << "SmartSys Agent (console)\n";
    std::cout << "Available modules:";
    for (auto &n : dm.listDevices()) std::cout << " " << n;
    std::cout << "\n\nExample commands:\n"
              << "  SetBrightness 60\n"
              << "  ShowBattery\n"
              << "  SetVolume 30\n"
              << "  GetVolume\n"
              << "  SetWiFi On\n"
              << "  SetBluetooth Off\n"
              << "  PlayMusic C:\\\\path\\\\song.wav\n"
              << "  StopMusic\n"
              << "  OpenApp \"VisualStudio.exe\"  (searches common folders)\n"
              << "  OpenPDF \"report.pdf\"\n"
              << "  Speak \"Hello team\"\n"
              << "Type 'exit' to quit.\n\n";

    std::string line;
    while (true) {
        std::cout << "Command> ";
        if (!std::getline(std::cin, line)) break;
        auto [action, args] = parseCommand(line);
        if (action.empty()) continue;
        if (action=="exit" || action=="quit") break;
        bool ok = dm.dispatch(action, args);
        if (!ok) std::cout << "[Core] Unknown command: " << action << "\n";
    }

    std::cout << "Goodbye!\n";
    return 0;
}
