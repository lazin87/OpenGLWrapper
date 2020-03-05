#pragma once

namespace RL {

namespace GL {

enum class ImageType: int
{
    INVALID = -1,
    PNG = 0,
    JPEG
};

struct Image {
    unsigned char* data;
    ImageType type;
    int width;
    int height;
    int channelsNbr;
};

bool isValid(const Image& image);

} // namespace GL

} // namespace RL
