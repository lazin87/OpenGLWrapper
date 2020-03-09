#pragma once

#include "Image.h"

#include <string>

namespace RL {

namespace GL {

enum class TextureType: int
{
    Common,
    Diffuse,
    Specular
};

class Texture2D
{
public:
    Texture2D() = default;
    Texture2D(const Image& image);

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    Texture2D(Texture2D&& other);
    Texture2D& operator=(Texture2D&& other);

    ~Texture2D();

    void use(const int index) const;
    unsigned int getId() const noexcept;
    TextureType getType() const noexcept;

    friend void swap(Texture2D& first, Texture2D& second) noexcept;

private:
    unsigned int m_glId {0};
    int m_width {-1};
    int m_height {-1};
    int m_channelsNbr {-1};
    TextureType m_type {TextureType::Common};
};

void swap(Texture2D& first, Texture2D& second) noexcept;
Texture2D loadTexture2D(const std::string& path, const TextureType type = TextureType::Common);

} // namespace GL

} // namespace RL
