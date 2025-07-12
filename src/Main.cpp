#include "VK/Window/Window.hpp"
#include <chrono>
#include <thread>

int main(void)
{
    constexpr vk::maths::Vector2u window_size(800, 600);
    vk::Window window(window_size, "VKE Window");

    while (true) {
        window.display();
        window.event();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    return 0;
}
