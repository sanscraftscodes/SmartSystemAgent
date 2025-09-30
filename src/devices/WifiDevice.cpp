#include "WifiDevice.h"
#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <iostream>
#include <string>
#pragma comment(lib, "wlanapi.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")

// helper to run an elevated command (will show UAC), returns true on success
static bool runElevatedProgramW(const std::wstring &program, const std::wstring &params) {
    HINSTANCE h = ShellExecuteW(NULL, L"runas", program.c_str(), params.c_str(), NULL, SW_SHOWNORMAL);
    return ((intptr_t)h > 32);
}

// returns primary interface name or empty
static std::wstring getPrimaryWlanInterfaceName() {
    HANDLE hClient = NULL;
    DWORD version = 0;
    if (WlanOpenHandle(2, NULL, &version, &hClient) != ERROR_SUCCESS) return L"";
    PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
    if (WlanEnumInterfaces(hClient, NULL, &pIfList) != ERROR_SUCCESS) { WlanCloseHandle(hClient, NULL); return L""; }
    if (pIfList->dwNumberOfItems < 1) { if (pIfList) WlanFreeMemory(pIfList); WlanCloseHandle(hClient, NULL); return L""; }
    std::wstring name = pIfList->InterfaceInfo[0].strInterfaceDescription;
    WlanFreeMemory(pIfList);
    WlanCloseHandle(hClient, NULL);
    return name;
}

bool WifiDevice::handles(const std::string &action) const {
    return action == "SetWiFi" || action == "ToggleWiFi";
}

void WifiDevice::operate(const std::string &action, const std::string &args) {
    // prefer explicit args: "On" or "Off"
    bool wantEnable = true;
    if (action == "SetWiFi") {
        if (args.empty()) { std::cout << "[WiFi] Usage: SetWiFi On|Off\n"; return; }
        std::string a = args;
        for (auto &c : a) c = tolower(c);
        if (a == "on" || a == "enable" || a=="1") wantEnable = true;
        else wantEnable = false;
    } else { // ToggleWiFi: try to enable if disabled or disable if enabled — we will just prompt user for On/Off for safety
        std::cout << "[WiFi] Use SetWiFi On|Off for deterministic behavior.\n";
        return;
    }

    std::wstring iface = getPrimaryWlanInterfaceName();
    if (iface.empty()) {
        std::wcout << L"[WiFi] Cannot find WLAN interface via WlanAPI. Will attempt to use default name 'Wi-Fi' via netsh.\n";
        iface = L"Wi-Fi";
    }

    // build netsh parameters: interface set interface name="Wi-Fi" admin=disable/enable
    std::wstring params = L"interface set interface name=\"" + iface + L"\" admin=" + (wantEnable ? L"enable" : L"disable");
    // Run elevated to allow admin change
    bool ok = runElevatedProgramW(L"netsh.exe", params);
    if (ok) std::wcout << L"[WiFi] Requested to " << (wantEnable ? L"enable " : L"disable ") << iface << L".\n";
    else std::cout << "[WiFi] Failed to launch elevated netsh. You may need to run this program as Administrator.\n";
}
