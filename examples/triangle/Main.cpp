#include "TriangleApplication.hpp"

#include <iostream>

int main()
{
    try {
        TriangleApplication app;
        app.run();
    } catch (const std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
