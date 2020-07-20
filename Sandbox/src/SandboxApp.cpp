#include <Hazel.h>

class SandBoxLayer : public Hazel::Layer {
public:
	SandBoxLayer() : Layer("Sandbox layer") {}
	~SandBoxLayer() {}

	void OnEvent(Hazel::Event& event) override {
		HZ_INFO("{0}", event);
	}
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	Sandbox* sandBox = new Sandbox();
	SandBoxLayer* sbLayer = new SandBoxLayer();
	ImGuiLayer* imgLayer = new ImGuiLayer();
	
	sandBox->PushLayer(sbLayer);
	sandBox->PushOverLay(imgLayer);

	return sandBox;
}