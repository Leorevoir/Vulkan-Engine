#pragma once

#include <VKE/Macros.hpp>
#include <VKE/Systems/EventTypes.hpp>
#include <VKE/Types.hpp>

namespace vke {

namespace event {

class VKE_API MouseEvent final : public NonCopyable
{
    public:
        static MouseEvent &getInstance();

        enum class Type { Left, Right, Middle, None };
        enum class Scroll { Up, Down, None };

        /** getters */
        const MousePosition &getPosition() const;
        const MouseButton &getButton() const;
        const MouseScroll &getScroll() const;

        /** setters */
        void setPosition(const MousePosition &position);
        void setButton(const Type &button_type);
        void setButton(const MouseButton &button);
        void setScroll(const Scroll &scroll);
        void setScroll(const MouseScroll &scroll);

    private:
        MousePosition _position;
        MouseButton _button;
        MouseScroll _scroll;
};

}// namespace event

}// namespace vke
