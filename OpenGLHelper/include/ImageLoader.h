#pragma once

#include "Image.h"

#include <string>
#include <optional>

namespace RL {

namespace GL {

class ImageLoader
{
public:
    ImageLoader() = default;
    ~ImageLoader();

    ImageLoader(const ImageLoader&) = delete;
    ImageLoader& operator=(const ImageLoader&) = delete;
    ImageLoader(ImageLoader&&) = delete;
    ImageLoader& operator=(ImageLoader&&) = delete;

    std::optional<Image> load(const std::string& path);

private:
    Image m_image {
        nullptr              // data
        , ImageType::INVALID // type
        , -1                 // width
        , -1                 // height
        , -1                 // channelsNbr
    };
};

} // namespace GL

} // namespace RL
