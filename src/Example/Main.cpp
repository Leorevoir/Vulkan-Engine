#include <Example/CubeEngine.hpp>
#include <VKE/Error.hpp>

#include <iostream>

int main(void)
{
    try {
        auto engine = std::make_unique<CubeEngine>();

        engine->start();
        engine->renderLoop();

    } catch (const vke::exception::Error &e) {
        std::cout << "Error: " << e.where() << std::endl << "Message: " << e.what() << std::endl;
    }

    return 0;
}
