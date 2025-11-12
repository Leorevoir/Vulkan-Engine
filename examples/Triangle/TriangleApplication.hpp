#pragma once

#include <lumen/Application.hpp>

class TriangleApplication : public lumen::Application
{
    protected:
        void on_create() override;
        void on_update() override;
        void on_destroy() override;

    private:
        std::unique_ptr<lumen::Pipeline> _pipeline;
};
