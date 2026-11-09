#ifndef UTILS_H
#define UTILS_H

#include <string>

// Small string helpers used across the project.
// Wrapped in a namespace so names like "trim" can't clash with other code.
namespace utils {

std::string toLower(const std::string &s);
std::string trim(const std::string &s);
bool containsIgnoreCase(const std::string &text, const std::string &keyword);

} // namespace utils

#endif
