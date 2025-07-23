#include <VKE/Error.hpp>
#include <VKE/Graphics/Shader/UniformCamera.hpp>
#include <VKE/Maths/Maths.hpp>

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
    updateThirdPersonCamera();
    updateCameraMatrix();
}

/**
 * protected
 */

void vke::UniformCamera::updateCameraMatrix()
{
    constexpr f32 radians = maths::radians(TEST_FOV);
    const f32 ratio = static_cast<f32>(_context->_screen_size->width) / static_cast<f32>(_context->_screen_size->height);

    _camera._projection = maths::perspective(radians, ratio, 0.001f, 256.f);
    _camera._view = maths::translate(maths::Matrix4f::Identity(), maths::Vector3f(0.f, 0.f, -_camera_zoom));

    _camera._model = maths::translate(maths::Matrix4f::Identity(), _camera_position);
    _camera._model = maths::rotate(_camera._model, maths::radians(-_camera_rotation.x), maths::Vector3f(1.f, 0.f, 0.f));
    _camera._model = maths::rotate(_camera._model, maths::radians(_camera_rotation.y), maths::Vector3f(0.f, 1.f, 0.f));
    _camera._model = maths::rotate(_camera._model, maths::radians(_camera_rotation.z), maths::Vector3f(0.f, 0.f, 1.f));

    _camera._normal = maths::inverseTranspose(_camera._view * _camera._model);
    std::memcpy(_uniform_buffer._mapped, &_camera, sizeof(_camera));
}

/**
* private
*/
