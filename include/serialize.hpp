#include <vector>

std::vector<char> serialize_pairs(const std::vector<std::pair<int, int>>& coords);
std::vector<std::pair<int, int>> deserialize_pairs(const std::vector<char>& buffer);