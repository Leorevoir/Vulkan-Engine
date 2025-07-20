#pragma once

#include <VKE/Graphics/Engine/VulkanEngineBase.hpp>
#include <VKE/Macros.hpp>

namespace vke {

class VKE_API GameEngine : public priv::VulkanEngineBase
{
    public:
        GameEngine() = default;
        virtual ~GameEngine() = default;

        void update() override;

    private:
};

}// namespace vke
