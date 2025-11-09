#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace Filepath {

using Path = std::filesystem::path;

Path get(const std::string &base_path);
Path get_absolute(const std::string &path_from_root);

bool exists(const std::string &base_path) noexcept;

std::vector<char> read(const std::string &base_path);

void write(const std::string &base_path, const std::string &data);

void append(const std::string &base_path, const std::string &data);

void mkdir(const std::string &dir_path);

uintmax_t size(const std::string &base_path);

std::vector<Path> list(const std::string &dir_path);

bool remove(const std::string &base_path) noexcept;

}// namespace Filepath
