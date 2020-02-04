#include "Texture2D.h"

#include "Logger.h"
#include "Utilities.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <map>

namespace RL {

namespace GL {

int toGLPixelDataFormat(const ImageFileFormat format)
{
    switch(format)
    {
    case ImageFileFormat::PNG:
        return GL_RGBA;
    case ImageFileFormat::JPEG:
        return GL_RGB;
    default:
        return GL_INVALID_ENUM;
    }
}

int toGLPixelDataType(const ImageFileFormat format)
{
    switch(format)
    {
    case ImageFileFormat::PNG:
    case ImageFileFormat::JPEG:
        return GL_UNSIGNED_BYTE;
    default:
        return GL_INVALID_ENUM;
    }
}

ImageFileFormat getImageFileFormat(const std::string &path)
{
    static const std::map<std::string, ImageFileFormat> stringToImageFormat{
          {"png", ImageFileFormat::PNG}
        , {"jpg", ImageFileFormat::JPEG}
        , {"jpeg", ImageFileFormat::JPEG}
    };

    auto imageFormatItr = stringToImageFormat.find(getFileExtension(path));
    if(imageFormatItr == std::cend(stringToImageFormat))
    {
        logError(__FUNCTION__, "Can not determine the image file format or it is not supported: " + path);
        return ImageFileFormat::INVALID;
    }
    else
    {
        return imageFormatItr->second;
    }
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_glId);
}

void Texture2D::load(const std::string &path)
{
    m_path = path;
    const auto imageFormat = getImageFileFormat(m_path);
    auto *imageData = stbi_load(m_path.c_str(), &m_width, &m_height, &m_channelsNbr, 0);
    if((imageData == nullptr) || (imageFormat == ImageFileFormat::INVALID))
    {
        logError(__FUNCTION__, "Can not load texture: " + m_path);
        return;
    }

    glGenTextures(1, &m_glId);
    glBindTexture(GL_TEXTURE_2D, m_glId);
    glTexImage2D( GL_TEXTURE_2D
                , 0
                , GL_RGB
                , m_width
                , m_height
                , 0
                , toGLPixelDataFormat(imageFormat)
                , toGLPixelDataType(imageFormat)
                , imageData);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture2D::getId() const noexcept
{
    return m_glId;
}

} // namespace GL

} // namespace RL
