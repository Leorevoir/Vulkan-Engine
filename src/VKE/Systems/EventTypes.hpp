#pragma once

#include <VKE/Maths/Vector.hpp>

namespace vke {

namespace event {

struct VKE_API MouseButton {

        bool left = false;
        bool right = false;
        bool middle = false;
};

struct VKE_API MouseScroll {

        bool up = false;
        bool down = false;
};

using MousePosition = maths::Vector2f;

}// namespace event

}// namespace vke
