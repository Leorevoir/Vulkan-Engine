#pragma once

namespace lumen {

class Application
{
    public:
        Application() = default;
        virtual ~Application() = default;

        virtual void run();

    private:
};

}// namespace lumen
