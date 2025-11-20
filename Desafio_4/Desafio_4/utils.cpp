#include "Utils.h"
#include <sstream>
#include <algorithm>
#include <cctype>

std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim)) {
        out.push_back(item);
    }
    return out;
}

std::string trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end-1]))) --end;
    return s.substr(start, end-start);
}
