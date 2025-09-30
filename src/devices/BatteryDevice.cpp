#include "BatteryDevice.h"
#include <windows.h>
#include <iostream>

bool BatteryDevice::handles(const std::string &action) const {
    return action == "ShowBattery";
}

void BatteryDevice::operate(const std::string &action, const std::string &args) {
    SYSTEM_POWER_STATUS sps;
    if (GetSystemPowerStatus(&sps)) {
        int pct = (sps.BatteryLifePercent==255)? -1 : sps.BatteryLifePercent;
        std::string ac = (sps.ACLineStatus == 1) ? "Charging" : "On Battery";
        if (pct>=0) std::cout << "[Battery] "<< pct << "% ("<<ac<<")\n";
        else std::cout << "[Battery] Unknown battery level. (AC: "<<ac<<")\n";
    } else {
        std::cout << "[Battery] Failed to read battery status.\n";
    }
}
