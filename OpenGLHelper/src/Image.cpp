#include "Image.h"

namespace RL {

namespace GL {

bool isValid(const Image &image)
{
    return (image.data != nullptr)
        && (image.type != ImageType::INVALID);
}

} // namespace GL

} // namespace RL
