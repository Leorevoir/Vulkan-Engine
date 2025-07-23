#include <VKE/Scene/Camera/ThirdPersonCamera.hpp>
#include <VKE/Systems/MouseEvent.hpp>

/**
 * protected
*/

void vke::ThirdPersonCamera::updateThirdPersonCamera()
{
    const auto &mouse_event = vke::event::MouseEvent::getInstance();
    const auto &mouse_position = mouse_event.getPosition();
    const auto &mouse_scroll = mouse_event.getScroll();

    /** rotation */
    if (mouse_event.getButton().left) {

        if (mouse_position.x > _old_position.x) {
            _camera_rotation.y += 1.f;
        } else if (mouse_position.x < _old_position.x) {
            _camera_rotation.y -= 1.f;
        }
        if (mouse_position.y > _old_position.y) {
            _camera_rotation.x += 1.f;
        } else if (mouse_position.y < _old_position.y) {
            _camera_rotation.x -= 1.f;
        }
    }

    /** zoom */
    if (mouse_scroll.up) {
        _camera_distance += 0.1f;
    } else if (mouse_scroll.down) {
        _camera_distance -= 0.1f;
    }

    _camera_zoom += _camera_distance;
    _camera_distance = 0.f;

    _old_position = mouse_position;
}
