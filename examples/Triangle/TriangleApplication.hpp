#pragma once

#include <lumen/Application.hpp>
#include <vulkan_backend/pipeline/Pipeline.hpp>

#include <memory>

class TriangleApplication final : public lumen::Application
{
    public:
        TriangleApplication() = default;
        ~TriangleApplication() = default;

    protected:
        void on_create() override;
        void on_update() override;
        void on_destroy() override;

    private:
        std::unique_ptr<lumen::Pipeline> _pipeline;
};
