#pragma once

#include <VK/Types.hpp>

#include <exception>
#include <string>

namespace vk::exception {

class Error : public std::exception
{
    public:
        VK_NODISCARD VK_INLINE explicit Error(const std::string &where, const std::string &what) : _where(where), _what(what)
        {
            /* __ctor__ */
        }

        VK_NODISCARD VK_INLINE const char *what() const noexcept override
        {
            return _what.c_str();
        }
        VK_NODISCARD VK_INLINE const char *where() const noexcept
        {
            return _where.c_str();
        }

    private:
        std::string _where;
        std::string _what;
};

class InvalidArgument final : public Error
{
    public:
        VK_NODISCARD VK_INLINE explicit InvalidArgument(const std::string &where, const std::string &what) : Error(where, what)
        {
            /* __ctor__ */
        }
};

class RuntimeError final : public Error
{
    public:
        VK_NODISCARD VK_INLINE explicit RuntimeError(const std::string &where, const std::string &what) : Error(where, what)
        {
            /* __ctor__ */
        }
};

class NotImplemented final : public Error
{
    public:
        VK_NODISCARD VK_INLINE explicit NotImplemented(const std::string &where, const std::string &what) : Error(where, what)
        {
            /* __ctor__ */
        }
};

}// namespace vk::exception
