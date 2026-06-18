#pragma once

#include "Engine/Math/Vec3.h"

#include <string>
#include <string_view>
#include <vector>

namespace rw::game::data {

std::string Trim(std::string_view text);
std::vector<std::string> Split(std::string_view value, char delimiter);
bool IsCommentOrBlankLine(std::string_view line);
std::string ReadFileText(const std::string& path, bool& outSuccess);
std::string MakeLineError(std::string_view domain, int lineNumber, std::string_view message);
bool ParsePositiveInt(std::string_view text, int& outValue);
bool ParsePositiveFloat(std::string_view text, float& outValue);
bool ParseVec3Triple(std::string_view text, rw::math::Vec3& outValue);

} // namespace rw::game::data
