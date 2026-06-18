#include "Game/Source/Data/DataTextParser.h"

#include <cctype>
#include <cstddef>
#include <fstream>
#include <sstream>

namespace rw::game::data {

std::string Trim(std::string_view text)
{
    std::size_t first = 0;
    while (first < text.size() && std::isspace(static_cast<unsigned char>(text[first])) != 0) {
        ++first;
    }

    std::size_t last = text.size();
    while (last > first && std::isspace(static_cast<unsigned char>(text[last - 1])) != 0) {
        --last;
    }

    return std::string(text.substr(first, last - first));
}

std::vector<std::string> Split(std::string_view value, char delimiter)
{
    std::vector<std::string> parts;
    std::string text(value);
    std::stringstream stream(text);
    std::string part;
    while (std::getline(stream, part, delimiter)) {
        parts.push_back(Trim(part));
    }
    return parts;
}

bool IsCommentOrBlankLine(std::string_view line)
{
    const std::string trimmed = Trim(line);
    return trimmed.empty() || trimmed[0] == '#';
}

std::string ReadFileText(const std::string& path, bool& outSuccess)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        outSuccess = false;
        return {};
    }

    std::ostringstream text;
    text << file.rdbuf();
    outSuccess = true;
    return text.str();
}

std::string MakeLineError(std::string_view domain, int lineNumber, std::string_view message)
{
    return std::string(domain) + " data line " + std::to_string(lineNumber) + ": " + std::string(message);
}

bool ParsePositiveInt(std::string_view text, int& outValue)
{
    try {
        outValue = std::stoi(Trim(text));
    } catch (...) {
        return false;
    }
    return outValue > 0;
}

bool ParsePositiveFloat(std::string_view text, float& outValue)
{
    try {
        outValue = std::stof(Trim(text));
    } catch (...) {
        return false;
    }
    return outValue > 0.0F;
}

bool ParseVec3Triple(std::string_view text, rw::math::Vec3& outValue)
{
    const std::vector<std::string> parts = Split(text, ',');
    if (parts.size() != 3) {
        return false;
    }

    try {
        outValue = {
            std::stof(parts[0]),
            std::stof(parts[1]),
            std::stof(parts[2]),
        };
    } catch (...) {
        return false;
    }

    return outValue.x > 0.0F && outValue.y > 0.0F && outValue.z > 0.0F;
}

} // namespace rw::game::data
