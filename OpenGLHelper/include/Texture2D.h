#pragma once

#include <string>

namespace RL {

namespace GL {

enum class ImageFileFormat: int
{
    INVALID = -1,
    PNG = 0,
    JPEG
};

class Texture2D
{
public:
    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;
    Texture2D(Texture2D&&) = delete;
    Texture2D& operator=(Texture2D&&) = delete;

    Texture2D() = default;
    ~Texture2D();

    void load(const std::string &path);
    unsigned int getId() const noexcept;

private:
    unsigned int m_glId {0};
    std::string m_path;
    int m_width {-1};
    int m_height {-1};
    int m_channelsNbr {-1};
};

} // namespace GL

} // namespace RL
