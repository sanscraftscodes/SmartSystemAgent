#pragma once
#include "../../include/Device.h"

class MusicPlayerDevice : public Device {
public:
    bool handles(const std::string &action) const override;
    void operate(const std::string &action, const std::string &args) override;
    std::string name() const override { return "MusicPlayer"; }
};
