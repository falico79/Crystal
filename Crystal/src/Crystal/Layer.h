#pragma once

#include "Crystal/Core.h"
#include "Crystal/Events/Event.h"
#include "Crystal/Core/Timestep.h"

namespace Crystal {

	class CRYSTAL_API Layer 
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRenderer() {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};
}