#include "string_manip.hpp"
std::vector<std::string> split(const std::string& str, char delim) {
    std::vector<std::string> ret;
    std::string::const_iterator first  = str.cbegin();
    std::string::const_iterator second = str.cbegin();

    for(; second <= str.end(); ++second) {
        if(*(second) == delim || second == str.end()) {
            if(second != first) {
                ret.emplace_back(first, second);
            }
            first = second + 1;
        }
    }

    return ret;
}