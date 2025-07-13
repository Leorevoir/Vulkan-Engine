#pragma once

#include <VKE/Macros.hpp>

namespace vke {

namespace priv {

class VKE_HIDDEN RenderEngine
{
    public:
        virtual ~RenderEngine() = default;

        virtual void init() = 0;
        virtual void start() = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
};

}// namespace priv

}// namespace vke
