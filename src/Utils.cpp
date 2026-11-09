#include "Utils.h"
#include <algorithm>
#include <cctype>

namespace utils {

std::string toLower(const std::string &s) {
  std::string result = s;
  std::transform(result.begin(), result.end(), result.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
  return result;
}

std::string trim(const std::string &s) {
  const char *whitespace = " \t\r\n";
  std::size_t start = s.find_first_not_of(whitespace);
  if (start == std::string::npos) {
    return "";
  }
  std::size_t end = s.find_last_not_of(whitespace);
  return s.substr(start, end - start + 1);
}

bool containsIgnoreCase(const std::string &text, const std::string &keyword) {
  return toLower(text).find(toLower(keyword)) != std::string::npos;
}

} // namespace utils
