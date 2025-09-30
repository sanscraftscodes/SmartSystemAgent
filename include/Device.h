#pragma once
#include <string>

class Device {
public:
    virtual ~Device() = default;
    // Check whether this device can handle the given action (e.g., "SetBrightness", "Speak").
    virtual bool handles(const std::string& action) const = 0;
    // Perform the operation (action and free-form args)
    virtual void operate(const std::string& action, const std::string& args) = 0;
    virtual std::string name() const = 0;
};
