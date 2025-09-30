#include "BrightnessDevice.h"
#include <iostream>

bool BrightnessDevice::handles(const std::string &action) const {
    // Return true if this device can handle the given action
    return action == "brightness" || action == "set_brightness";
}

void BrightnessDevice::operate(const std::string &action, const std::string &args) {
    // Implement the action here
    // For now, we just print to console
    std::cout << "[BrightnessDevice] Action: " << action 
              << ", Args: " << args << std::endl;

    // TODO: Add real brightness adjustment logic if hardware is present
}



