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

GLenum toGLTexture(const int idx)
{
    switch(idx)
    {
    case 0:
        return GL_TEXTURE0;
    case 1:
        return GL_TEXTURE1;
    case 2:
        return GL_TEXTURE2;
    case 3:
        return GL_TEXTURE3;
    case 4:
        return GL_TEXTURE4;
    case 5:
        return GL_TEXTURE5;

    default:
        return GL_INVALID_ENUM;
    }
};
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

void Texture2D::use(const int index)
{
    glActiveTexture(toGLTexture(index));
    glBindTexture(GL_TEXTURE_2D, m_glId);
}

unsigned int Texture2D::getId() const noexcept
{
    return m_glId;
}

} // namespace GL

} // namespace RL
