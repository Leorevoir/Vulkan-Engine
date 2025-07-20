#include <VKE/Error.hpp>
#include <VKE/Graphics/Shader/UniformCamera.hpp>

#define TEST_FOV 60.f

/**
* public
*/

void vke::UniformCamera::initialize()
{
    VKE_ASSERT(_context->_device->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        &_uniform_buffer, sizeof(_camera), &_camera));
    VKE_ASSERT(_uniform_buffer.map());
    update();
}

void vke::UniformCamera::update()
{
    const f32 fov = VKEM_DEG2RAD(TEST_FOV);
    const f32 aspect_ratio = static_cast<f32>(_context->_screen_size->width) / static_cast<f32>(_context->_screen_size->height);
    const f32 near_plane = 0.001f;
    const f32 far_plane = 256.0f;

    _camera._projection = maths::perspective(fov, aspect_ratio, near_plane, far_plane);
    _camera._view = maths::translate(maths::Matrix4f(1.f), maths::Vector3f(0.f, 0.f, -*_zoom));

    maths::Matrix4f model = maths::translate(maths::Matrix4f(1.f), *_position);

    model = maths::rotate(model, VKEM_DEG2RAD(-_rotation->x), maths::Vector3f(1.f, 0.f, 0.f));
    model = maths::rotate(model, VKEM_DEG2RAD(_rotation->y), maths::Vector3f(0.f, 1.f, 0.f));
    model = maths::rotate(model, VKEM_DEG2RAD(_rotation->z), maths::Vector3f(0.f, 0.f, 1.f));

    _camera._model = model;
    _camera._normal = maths::inverseTranspose(_camera._view * model);
    std::memcpy(_uniform_buffer._mapped, &_camera, sizeof(_camera));
}

/**
 * setters
 */

void vke::UniformCamera::setPosition(maths::Vector3f *position)
{
    if (position) {
        _position = position;
    }
}

void vke::UniformCamera::setRotation(maths::Vector3f *rotation)
{
    if (rotation) {
        _rotation = rotation;
    }
}

void vke::UniformCamera::setZoom(f32 *zoom)
{
    if (zoom) {
        _zoom = zoom;
    }
}

/**
 * protected
 */

/**
* private
*/
