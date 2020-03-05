#include "ImageLoader.h"

#include "Logger.h"
#include "Utilities.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <map>

namespace RL {

namespace GL {

namespace  {

ImageType getImageType(const std::string &path)
{
    static const std::map<std::string, ImageType> stringToImageFormat{
          {"png", ImageType::PNG}
        , {"jpg", ImageType::JPEG}
        , {"jpeg", ImageType::JPEG}
    };

    auto imageFormatItr = stringToImageFormat.find(getFileExtension(path));
    if(imageFormatItr == std::cend(stringToImageFormat))
    {
        logError(__FUNCTION__, "Can not determine the image file format or it is not supported: " + path);
        return ImageType::INVALID;
    }
    else
    {
        return imageFormatItr->second;
    }
}

} // namespace

ImageLoader::~ImageLoader()
{
    if(m_image.data)
    {
        stbi_image_free(m_image.data);
    }
}

std::optional<Image> ImageLoader::load(const std::string &path)
{
    m_image.type = getImageType(path);
    m_image.data = stbi_load( path.c_str()
                            , &m_image.width
                            , &m_image.height
                            , &m_image.channelsNbr
                            , 0);

    if(!isValid(m_image))
    {
        logError(__FUNCTION__, "Can not load image: " + path);
        return {};
    }

    return {m_image};
}

} // namespace GL

} // namespace RL
