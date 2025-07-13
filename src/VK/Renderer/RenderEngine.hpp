#pragma once

#include <VK/Macros.hpp>

namespace vk {

namespace priv {

namespace render {

class VK_HIDDEN Engine
{
    public:
        virtual ~Engine() = default;

        virtual void init() = 0;
        virtual void start() = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
};

}// namespace render

}// namespace priv

}// namespace vk
