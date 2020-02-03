#include "Logger.h"

#include <iostream>

namespace RL {

void logError(const std::string& functionName, const std::string& msg)
{
    std::cout << "ERROR: " << functionName << ": " << msg << std::endl;
}

} // namespace RL
