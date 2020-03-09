#include "Texture2D.h"

#include "ImageLoader.h"
#include "Logger.h"

#include <glad/glad.h>

namespace RL {

namespace GL {

namespace  {

int toGLPixelDataFormat(const ImageType type)
{
    switch(type)
    {
    case ImageType::PNG:
        return GL_RGBA;
    case ImageType::JPEG:
        return GL_RGB;
    default:
        return GL_INVALID_ENUM;
    }
}

int toGLPixelDataType(const ImageType type)
{
    switch(type)
    {
    case ImageType::PNG:
    case ImageType::JPEG:
        return GL_UNSIGNED_BYTE;
    default:
        return GL_INVALID_ENUM;
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

} // namespace

Texture2D::Texture2D(Texture2D &&other)
    : Texture2D()
{
    *this = std::move(other);
}

Texture2D::Texture2D(const Image &image)
    : m_width {image.width}
    , m_height {image.height}
    , m_channelsNbr {image.channelsNbr}
{
    glGenTextures(1, &m_glId);
    glBindTexture(GL_TEXTURE_2D, m_glId);
    glTexImage2D( GL_TEXTURE_2D
                , 0
                , GL_RGB
                , m_width
                , m_height
                , 0
                , toGLPixelDataFormat(image.type)
                , toGLPixelDataType(image.type)
                , image.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D &Texture2D::operator=(Texture2D && other)
{
    swap(*this, other);
    return *this;
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &m_glId);
}

void Texture2D::use(const int index) const
{
    glActiveTexture(toGLTexture(index));
    glBindTexture(GL_TEXTURE_2D, m_glId);
}

unsigned int Texture2D::getId() const noexcept
{
    return m_glId;
}

TextureType Texture2D::getType() const noexcept
{
    return m_type;
}

Texture2D loadTexture2D(const std::string &path, const TextureType type)
{
    ImageLoader l;
    const auto image = l.load(path);
    if(!image) return Texture2D{};

    return Texture2D{image.value()};
}

void swap(Texture2D &first, Texture2D &second) noexcept
{
    using std::swap;

    swap(first.m_glId, second.m_glId);
    swap(first.m_width, second.m_width);
    swap(first.m_height, second.m_height);
    swap(first.m_channelsNbr, second.m_channelsNbr);
    swap(first.m_type, second.m_type);
}

} // namespace GL

} // namespace RL
