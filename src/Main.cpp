#include <VK/Window/Window.hpp>
#include <thread>

int main(void)
{
    const vk::maths::Vector2u size(800, 600);
    auto window = std::make_unique<vk::Window>(size, "Vulkan Window");

    while (!window->shouldClose()) {
        window->pollEvents();
        window->display();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}
