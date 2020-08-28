#include <Hazel.h>
#include "Hazel/Core/EntryPoint.h"
#include "EditorLayer.h"


namespace Hazel {

	class HazelEditor : public Application {
	public:
		HazelEditor(): Application("Hazel Editor") {

		}

		~HazelEditor() {

		}
	};

	Application* CreateApplication() {
		HazelEditor* sandBox = new HazelEditor();

		EditorLayer* layer2D = new EditorLayer();

		sandBox->PushLayer(layer2D);

		return sandBox;
	}
}