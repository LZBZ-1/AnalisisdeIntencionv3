//
// Created by jalb2 on 3/11/2023.
//

#include <string>
#include <vector>

#ifndef ANALISISDEINTENCIONV3_UTILS_H
#define ANALISISDEINTENCIONV3_UTILS_H

std::vector<std::u32string> split(const std::u32string& str, char32_t delimiter) {
    std::vector<std::u32string> result;
    std::u32string::size_type start = 0;
    std::u32string::size_type end = str.find(delimiter);
    while (end != std::u32string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    result.push_back(str.substr(start));
    return result;
}

int search(const std::vector<std::u32string>& vec, const std::u32string& str) {
    auto it = std::lower_bound(vec.begin(), vec.end(), str);
    if (it != vec.end() && *it == str) {
        return std::distance(vec.begin(), it);
    } else {
        return -1;
    }
}

#endif //ANALISISDEINTENCIONV3_UTILS_H
