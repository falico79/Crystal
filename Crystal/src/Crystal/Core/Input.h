#pragma once

#include "Crystal/Core/Core.h"
#include "Crystal/Core/KeyCodes.h"
#include "Crystal/Core/MouseCodes.h"

namespace Crystal {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}