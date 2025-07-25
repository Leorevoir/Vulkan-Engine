#include <VKE/Systems/MouseEvent.hpp>

/**
* public
*/

vke::event::MouseEvent &vke::event::MouseEvent::getInstance()
{
    static MouseEvent instance;

    return instance;
}

/**
* getters
*/

vke::event::MousePosition const &vke::event::MouseEvent::getPosition() const
{
    return _position;
}

vke::event::MouseButton const &vke::event::MouseEvent::getButton() const
{
    return _button;
}

vke::event::MouseScroll const &vke::event::MouseEvent::getScroll() const
{
    return _scroll;
}

/**
* setters
*/

void vke::event::MouseEvent::setPosition(const MousePosition &position)
{
    _position = position;
}

void vke::event::MouseEvent::setButton(const Type &button_type)
{
    switch (button_type) {
        case Type::Left:
            _button.left = !_button.left;
            break;
        case Type::Right:
            _button.right = !_button.right;
            break;
        case Type::Middle:
            _button.middle = !_button.middle;
            break;
        case Type::None:
            break;
        default:
            break;
    }
}
void vke::event::MouseEvent::setButton(const MouseButton &button)
{
    _button = button;
}

void vke::event::MouseEvent::setScroll(const Scroll &scroll)
{
    switch (scroll) {
        case Scroll::Up:
            _scroll.up = !_scroll.up;
            break;
        case Scroll::Down:
            _scroll.down = !_scroll.down;
            break;
        case Scroll::None:
            break;
        default:
            break;
    }
}

void vke::event::MouseEvent::setScroll(const MouseScroll &scroll)
{
    _scroll = scroll;
}
