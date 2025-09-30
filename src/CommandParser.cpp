#include "../include/CommandParser.h"
#include <sstream>
#include <algorithm>

static inline std::string trim(const std::string &s) {
    auto a = s.find_first_not_of(" \t\n\r");
    if (a==std::string::npos) return "";
    auto b = s.find_last_not_of(" \t\n\r");
    return s.substr(a, b-a+1);
}

std::pair<std::string, std::string> parseCommand(const std::string &input) {
    std::string in = trim(input);
    if (in.empty()) return {"",""};
    std::istringstream iss(in);
    std::string action;
    if (!(iss >> action)) return {"",""};
    std::string rest;
    std::getline(iss, rest);
    rest = trim(rest);
    // If rest starts with quote, extract between quotes
    if (!rest.empty() && rest.front()=='\"') {
        auto firstQuote = in.find('\"');
        auto secondQuote = in.find('\"', firstQuote+1);
        if (firstQuote!=std::string::npos && secondQuote!=std::string::npos && secondQuote>firstQuote)
            rest = in.substr(firstQuote+1, secondQuote-firstQuote-1);
    }
    return {action, rest};
}
