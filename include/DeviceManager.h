#pragma once
#include "Device.h"
#include <vector>
#include <memory>
#include <string>

class DeviceManager {
    std::vector<std::unique_ptr<Device>> devices;
public:
    void registerDevice(std::unique_ptr<Device> d) { devices.push_back(std::move(d)); }

    // Dispatch action to the first device that handles it
    bool dispatch(const std::string& action, const std::string& args) {
        for (auto &d : devices) {
            if (d->handles(action)) {
                d->operate(action, args);
                return true;
            }
        }
        return false;
    }

    std::vector<std::string> listDevices() const {
        std::vector<std::string> out;
        for (auto &d : devices) out.push_back(d->name());
        return out;
    }
};
