#include <VKE/Core/Time.hpp>
#include <VKE/Maths/Maths.hpp>
#include <VKE/Scene/Camera/ThirdPersonCamera.hpp>
#include <VKE/Systems/MouseEvent.hpp>

/** TODO: make parameters / setters configurable */
#define TPC_ROTATION_SENSITIVITY 150.f
#define TPC_ROTATION_SMOOTHING 8.f
#define TPC_ZOOM_SENSITIVITY 2.f
#define TPC_ZOOM_SMOOTHING 5.f
#define TPC_MAX_VERTICAL_ANGLE 89.f

/**
 * protected
*/

void vke::ThirdPersonCamera::updateThirdPersonCamera()
{
    const f32 delta_time = core::Time::getInstance().getDeltaTime();

    _third_person_camera_rotation(delta_time);
    _third_person_camera_interpolation(delta_time);
    _third_person_camera_zoom(delta_time);
}

/**
* private
*/

void vke::ThirdPersonCamera::_third_person_camera_rotation(const f32 dt)
{
    const auto &mouse_event = vke::event::MouseEvent::getInstance();

    if (!mouse_event.getButton().left) {
        return;
    }

    const auto &mouse_position = mouse_event.getPosition();
    const float delta_x = mouse_position.x - _old_position.x;
    const float delta_y = mouse_position.y - _old_position.y;

    _target_rotation.y += delta_x * TPC_ROTATION_SENSITIVITY * dt;
    _target_rotation.x += delta_y * TPC_ROTATION_SENSITIVITY * dt;

    _target_rotation.x = maths::clamp(_target_rotation.x, -TPC_MAX_VERTICAL_ANGLE, TPC_MAX_VERTICAL_ANGLE);
}

void vke::ThirdPersonCamera::_third_person_camera_interpolation(const f32 dt)
{
    _camera_rotation.x = maths::lerp(_camera_rotation.x, _target_rotation.x, TPC_ROTATION_SMOOTHING * dt);
    _camera_rotation.y = maths::lerp(_camera_rotation.y, _target_rotation.y, TPC_ROTATION_SMOOTHING * dt);
}

void vke::ThirdPersonCamera::_third_person_camera_zoom(const f32 dt)
{
    const auto &mouse_event = vke::event::MouseEvent::getInstance();
    const auto &mouse_scroll = mouse_event.getScroll();
    f32 zoom_input = 0.f;

    if (mouse_scroll.up) {
        zoom_input = TPC_ZOOM_SENSITIVITY * dt;
    } else if (mouse_scroll.down) {
        zoom_input = -TPC_ZOOM_SENSITIVITY * dt;
    }

    _target_zoom += zoom_input;
    _target_zoom = maths::clamp(_target_zoom, 1.f, 50.f);

    /** interpolate zoom */
    _camera_zoom = maths::lerp(_camera_zoom, _target_zoom, TPC_ZOOM_SMOOTHING * dt);
    _old_position = mouse_event.getPosition();
}
