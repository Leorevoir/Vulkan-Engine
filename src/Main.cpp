#include "Application.hpp"

#include <exception>
#include <iostream>

#define SUCCESS 0
#define ERROR 84

int main(void)
{
    try {
        lumen::Application app;
        app.run();

    } catch (const std::exception &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return ERROR;
    }
    return SUCCESS;
}
