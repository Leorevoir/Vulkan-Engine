#pragma once

#include <VKE/Macros.hpp>
#include <VKE/Maths/Matrix.hpp>

namespace vke {

struct VKE_API Camera {
        maths::Matrix4f _projection;
        maths::Matrix4f _model;
        maths::Matrix4f _view;
        maths::Matrix4f _normal;
        maths::Matrix4f _lightpos;
};

}// namespace vke
