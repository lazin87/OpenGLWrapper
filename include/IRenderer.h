#pragma once

namespace RL {

namespace GL {

class IRenderer
{
public:
    virtual ~IRenderer() = default;

    virtual void init() = 0;
    virtual void render() = 0;
};

} // namespace GL

} // namespace RL
