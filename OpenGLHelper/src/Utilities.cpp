#include "Utilities.h"

#include "Logger.h"

#include <algorithm>

namespace RL {

namespace GL {

std::string getFileExtension(const std::string &path)
{
    const auto extensionBeforeStart = path.rfind(".");
    if(extensionBeforeStart == std::string::npos)
    {
        logError(__FUNCTION__, "Extension not found: " + path);
        return "";
    }

    std::string extension = path.substr(extensionBeforeStart + 1U);
    std::transform(std::cbegin(extension), std::cend(extension), std::begin(extension),
                   [](const char c) { return std::tolower(c); } );

    return extension;
}

} // namespace GL

} // namespace RL
