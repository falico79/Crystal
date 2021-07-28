#pragma once

#include <glm/glm.hpp>

#include "Crystal/Core/KeyCodes.h"
#include "Crystal/Core/MouseCodes.h"

namespace Crystal {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
