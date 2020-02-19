#pragma once

#include <string>

namespace RL {

void logError(const std::string& functionName, const std::string& msg);
void logWarning(const std::string& functionName, const std::string& msg);

} // namespace RL
