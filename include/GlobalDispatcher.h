#pragma once
#include <string>

class DeviceManager;

void setGlobalDeviceManager(DeviceManager* mgr);
bool globalDispatch(const std::string &action, const std::string &args);
