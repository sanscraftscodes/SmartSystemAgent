#pragma once
#include "../../include/Device.h"
#include <atomic>

class VoiceAssistantDevice : public Device {
private:
    std::atomic<bool> voiceEnabled{false};

public:
    bool handles(const std::string &action) const override;
    void operate(const std::string &action, const std::string &args) override;
    std::string name() const override { return "VoiceAssistant"; }

    // 👇 new helper methods
    void startListening();
    void stopListening();
};
