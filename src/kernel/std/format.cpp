#include <format>

namespace std {

namespace internal {

std::string to_string(const char* value) {
    return value;
}

// std::string to_string(const std::string& value) {
//     return value;
// }

}  // namespace internal

}  // namespace std
