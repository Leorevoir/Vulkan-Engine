#include <utils/Filepath.hpp>

#include <fstream>
#include <stdexcept>

/**
* public
*/

#ifndef LUMEN_ROOT_DIR
    #define LUMEN_ROOT_DIR "."
    #warning "LUMEN_ROOT_DIR is not defined. Defaulting to current directory."
#endif

/**
* public
*/

std::filesystem::path Filepath::get(const std::string &base_path)
{
    const Path path = std::filesystem::absolute(base_path);

    return path.lexically_normal();
}

std::filesystem::path Filepath::get_absolute(const std::string &path_from_root)
{
    const Path root_dir = LUMEN_ROOT_DIR;

    return (root_dir / path_from_root).lexically_normal();
}

bool Filepath::exists(const std::string &base_path) noexcept
{
    try {
        return std::filesystem::exists(get(base_path));
    } catch (...) {
        return false;
    }
}

std::vector<char> Filepath::read(const std::string &base_path)
{
    const Path abs_path = get(base_path);
    std::ifstream file(abs_path, std::ios::ate | std::ios::binary);

    if (!file) {
        throw std::runtime_error("Cannot open file: " + abs_path.string());
    }

    const size_t file_size = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(file_size);

    file.seekg(0);
    file.read(buffer.data(), static_cast<std::streamsize>(file_size));
    file.close();

    return buffer;
}

std::string Filepath::read_str(const std::string &base_path)
{
    const auto bytes = Filepath::read(base_path);

    return std::string(bytes.begin(), bytes.end());
}

uintmax_t Filepath::size(const std::string &base_path)
{
    Path abs_path = get(base_path);

    if (!std::filesystem::exists(abs_path)) {
        throw std::runtime_error("File not found: " + abs_path.string());
    }
    return std::filesystem::file_size(abs_path);
}

std::vector<Filepath::Path> Filepath::list(const std::string &dir_path)
{
    const Path abs_dir = get(dir_path);

    if (!std::filesystem::exists(abs_dir) || !std::filesystem::is_directory(abs_dir)) {
        throw std::runtime_error("Not a valid directory: " + abs_dir.string());
    }

    std::vector<Path> entries;

    for (const auto &entry : std::filesystem::directory_iterator(abs_dir)) {
        entries.push_back(entry.path());
    }

    return entries;
}

void Filepath::write(const std::string &base_path, const std::string &data)
{
    const Path abs_path = get(base_path);
    std::ofstream file(abs_path, std::ios::binary | std::ios::trunc);

    if (!file) {
        throw std::runtime_error("Cannot write to file: " + abs_path.string());
    }
    file.write(data.data(), static_cast<std::streamsize>(data.size()));
}

void Filepath::append(const std::string &base_path, const std::string &data)
{
    const Path abs_path = get(base_path);
    std::ofstream file(abs_path, std::ios::binary | std::ios::app);

    if (!file) {
        throw std::runtime_error("Cannot append to file: " + abs_path.string());
    }
    file.write(data.data(), static_cast<std::streamsize>(data.size()));
}

void Filepath::mkdir(const std::string &dir_path)
{
    const Path dir = get(dir_path);

    if (!std::filesystem::exists(dir)) {

        if (!std::filesystem::create_directories(dir)) {
            throw std::runtime_error("Failed to create directory: " + dir.string());
        }
    }
}

bool Filepath::remove(const std::string &base_path) noexcept
{
    try {
        return std::filesystem::remove_all(get(base_path)) > 0;
    } catch (...) {
        return false;
    }
}
