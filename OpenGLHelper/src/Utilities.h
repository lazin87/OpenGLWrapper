#pragma once

#include <string>
#include <functional>

namespace RL {

namespace GL {

struct DefaultTag {};

template <typename T, typename TagType = DefaultTag>
struct CallableObjectToFunction;

template <typename RetType, typename... ParamsType, typename TagType>
struct CallableObjectToFunction<RetType(ParamsType...), TagType>
{
    static std::function<RetType(ParamsType...)> obj;

    template <typename... Args>
    static RetType func(Args... args)
    {
        return obj(args...);
    }
};


std::string getFileExtension(const std::string& path);

template <typename RetType, typename... ParamsType, typename TagType>
std::function<RetType(ParamsType...)> CallableObjectToFunction<RetType(ParamsType...), TagType>::obj;

} // namespace GL

} // namespace RL
