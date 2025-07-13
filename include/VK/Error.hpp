#pragma once

#include <VK/Types.hpp>

#include <exception>
#include <sstream>
#include <string>

#define __VK_ERROR_INHERIT(N)                                                                                                                                            \
    template<typename... Args>                                                                                                                                           \
    constexpr explicit N(std::string where, Args &&...args) : Error(std::move(where), std::forward<Args>(args)...)                                                       \
    {                                                                                                                                                                    \
        /* __ctor__ */                                                                                                                                                   \
    }

namespace vk::exception {

class Error : public std::exception
{
    public:
        template<typename... Args>
        constexpr explicit Error(std::string where, Args &&...args) : _where(std::move(where)), _what(_concat(std::forward<Args>(args)...))
        {
            /* __ctor__ */
        }

        [[nodiscard]] const char *what() const noexcept override
        {
            return _what.c_str();
        }

        [[nodiscard]] const char *where() const noexcept
        {
            return _where.c_str();
        }

    private:
        const std::string _where;
        const std::string _what;

        template<typename... Args>
        static std::string _concat(Args &&...args)
        {
            std::ostringstream oss;

            (oss << ... << args);
            return oss.str();
        }
};

class InvalidArgument final : public Error
{
    public:
        __VK_ERROR_INHERIT(InvalidArgument)
};

class RuntimeError final : public Error
{
    public:
        __VK_ERROR_INHERIT(RuntimeError)
};

class NotImplemented final : public Error
{
    public:
        __VK_ERROR_INHERIT(NotImplemented)
};

class BadAlloc final : public Error
{
    public:
        __VK_ERROR_INHERIT(BadAlloc)
};

}// namespace vk::exception
