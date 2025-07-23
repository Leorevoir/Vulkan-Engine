#include <VKE/Filename.hpp>
#include <VKE/Macros.hpp>

#include <filesystem>

const std::string vke::Filename::getPath(const std::string &name)
{
    const std::filesystem::path absolute = std::filesystem::absolute(VKE_DEFAULT_PATH + name);

    return absolute.string();
}

bool vke::Filename::exists(const std::string &name)
{
    return std::filesystem::exists(name);
}
