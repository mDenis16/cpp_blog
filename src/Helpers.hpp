#include <iostream>

namespace Helpers {
    inline constexpr std::uint64_t fnv1a_64(const char* str, std::size_t len) {
        std::uint64_t hash = 14695981039346656037ULL;  // FNV offset basis
        for (std::size_t i = 0; i < len; ++i) {
            hash ^= static_cast<std::uint64_t>(str[i]);
            hash *= 1099511628211ULL;  // FNV prime
        }
        return hash;
    }

    inline constexpr std::uint64_t operator"" _hash(const char* str, std::size_t len) {
        return fnv1a_64(str, len);
    }
}