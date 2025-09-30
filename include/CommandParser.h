#pragma once
#include <string>
#include <utility>

// parseCommand("SetBrightness 70") => {"SetBrightness", "70"}
// parseCommand("Speak \"Hello world\"") => {"Speak", "Hello world"}
std::pair<std::string, std::string> parseCommand(const std::string &input);
