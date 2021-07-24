
#include <Crystal.h>
#include <Crystal/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Crystal {
	class CrystalEditor : public Application {
	public:
		CrystalEditor()
			: Application("Crystal Editor")
		{
			// PushLayer(new ExampleLayer());
			PushLayer(new EditorLayer());
		}

		~CrystalEditor()
		{

		}

	};

	Application* CreateApplication()
	{
		return new CrystalEditor();
	}

}