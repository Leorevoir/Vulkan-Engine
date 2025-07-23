#pragma once

#include <string>

namespace vke {

namespace Filename {

[[nodiscard]] const std::string getPath(const std::string &name);
[[nodiscard]] bool exists(const std::string &path);

}// namespace Filename

}// namespace vke
