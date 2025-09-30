#include "BluetoothDevice.h"
#include <windows.h>
#include <shellapi.h>
#include <string>
#include <iostream>
#include <cctype>      // for tolower
#include <cwchar>      // for std::wstring

#pragma comment(lib, "shell32.lib")

// Helper function to run program with admin privileges
static bool runElevatedProgramW(const std::wstring &program, const std::wstring &params) {
    HINSTANCE h = ShellExecuteW(NULL, L"runas", program.c_str(), params.c_str(), NULL, SW_SHOWNORMAL);
    return ((intptr_t)h > 32);
}

bool BluetoothDevice::handles(const std::string &action) const {
    return action == "SetBluetooth" || action == "ToggleBluetooth";
}

void BluetoothDevice::operate(const std::string &action, const std::string &args) {
    bool wantEnable = true;

    if (action == "SetBluetooth") {
        if (args.empty()) { 
            std::cout << "[Bluetooth] Usage: SetBluetooth On|Off\n"; 
            return; 
        }
        std::string a = args;
        for (auto &c : a) c = static_cast<char>(std::tolower(c));
        wantEnable = (a == "on" || a == "enable" || a == "1");
    } else {
        std::cout << "[Bluetooth] Use SetBluetooth On|Off for deterministic behavior.\n";
        return;
    }

    // Build PowerShell command safely
    std::wstring cmd = L"-Command \"Get-PnpDevice -Class Bluetooth | ";
    if (wantEnable)
        cmd += L"Enable-PnpDevice -Confirm:$false";
    else
        cmd += L"Disable-PnpDevice -Confirm:$false";
    cmd += L"\"";

    bool ok = runElevatedProgramW(L"powershell.exe", cmd);

    if (ok)
        std::cout << "[Bluetooth] Requested change via PowerShell. You may be prompted by UAC.\n";
    else
        std::cout << "[Bluetooth] Failed to launch PowerShell elevated. Run as Administrator.\n";
}
