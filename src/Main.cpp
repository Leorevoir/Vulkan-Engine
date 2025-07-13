#include <VKE/Window/Window.hpp>
#include <thread>

int main(void)
{
    const vke::maths::Vector2u size(800, 600);
    auto window = std::make_unique<vke::Window>(size, "Vulkan Window");

    while (!window->shouldClose()) {
        window->pollEvents();
        window->flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
