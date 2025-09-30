#include "AppLauncherDevice.h"
#include <windows.h>
#include <shellapi.h>
#include <filesystem>
#include <iostream>
#include <vector>
#pragma comment(lib, "shell32.lib")

// Helper: recursively search a set of root directories for filename (first match)
static bool findFileByName(const std::wstring& name, const std::vector<std::wstring>& roots, std::wstring& outPath) {
    namespace fs = std::filesystem;
    for (auto &root : roots) {
        try {
            for (auto &p : fs::recursive_directory_iterator(root)) {
                try {
                    if (!p.is_regular_file()) continue;
                    if (p.path().filename() == name) {
                        outPath = p.path().wstring();
                        return true;
                    }
                } catch (...) { /* ignore inaccessible files */ }
            }
        } catch(...) { /* ignore inaccessible roots */ }
    }
    return false;
}

bool AppLauncherDevice::handles(const std::string &action) const {
    return action == "OpenApp" || action == "OpenPDF";
}

void AppLauncherDevice::operate(const std::string &action, const std::string &args) {
    if (args.empty()) { std::cout << "[AppLauncher] Provide app path/name or PDF filename.\n"; return; }
    std::wstring wargs;
    int size = MultiByteToWideChar(CP_UTF8, 0, args.c_str(), -1, NULL, 0);
    wargs.resize(size);
    MultiByteToWideChar(CP_UTF8, 0, args.c_str(), -1, &wargs[0], size);

    // If path exists, open directly
    if (std::filesystem::exists(std::filesystem::path(wargs))) {
        HINSTANCE res = ShellExecuteW(NULL, L"open", wargs.c_str(), NULL, NULL, SW_SHOWNORMAL);
        if ((intptr_t)res <= 32) std::cout << "[AppLauncher] Failed to open: " << args << "\n";
        else std::cout << "[AppLauncher] Opened: " << args << "\n";
        return;
    }

    // Otherwise search common locations for file (Desktop, Documents, Downloads, Program Files)
    std::vector<std::wstring> roots;
    wchar_t* userProfile = _wgetenv(L"USERPROFILE");
    if (userProfile) {
        std::wstring up(userProfile);
        roots.push_back(up + L"\\Desktop");
        roots.push_back(up + L"\\Documents");
        roots.push_back(up + L"\\Downloads");
    }
    roots.push_back(L"C:\\Program Files");
    roots.push_back(L"C:\\Program Files (x86)");

    std::wstring found;
    if (findFileByName(wargs, roots, found)) {
        HINSTANCE res = ShellExecuteW(NULL, L"open", found.c_str(), NULL, NULL, SW_SHOWNORMAL);
        if ((intptr_t)res <= 32) std::cout << "[AppLauncher] Failed to open found file.\n";
        else std::cout << "[AppLauncher] Opened: " << std::string(args) << "\n";
    } else {
        std::cout << "[AppLauncher] File not found in standard locations: " << std::string(args) << "\n";
    }
}
