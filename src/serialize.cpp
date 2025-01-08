#include <cstring>
#include "serialize.hpp"

std::vector<char> serialize_pairs(const std::vector<std::pair<int, int>>& coords)
{
    std::vector<char> buffer(coords.size() * 2 * sizeof(int));
    char* ptr = buffer.data();

    for (const auto& [x, y] : coords)
    {
        std::memcpy(ptr, &x, sizeof(int));
        ptr += sizeof(int);
        std::memcpy(ptr, &y, sizeof(int));
        ptr += sizeof(int);
    }

    return buffer;
}

std::vector<std::pair<int, int>> deserialize_pairs(const std::vector<char>& buffer)
{
    std::vector<std::pair<int, int>> coords(buffer.size() / (2 * sizeof(int)));
    const char* ptr = buffer.data();

    for (auto& [x, y] : coords)
    {
        std::memcpy(&x, ptr, sizeof(int));
        ptr += sizeof(int);
        std::memcpy(&y, ptr, sizeof(int));
        ptr += sizeof(int);
    }

    return coords;
}
