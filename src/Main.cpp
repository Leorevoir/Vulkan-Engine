#include "Application.hpp"

#define SUCCESS 0
#define ERROR 84

#include <exception>
#include <iostream>

int main(void)
{
    lumen::Application app;

    try {
        app.run();
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return ERROR;
    }
    return SUCCESS;
}
