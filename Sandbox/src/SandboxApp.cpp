#include <Hazel.h>
#include "imgui.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

class SandBoxLayer : public Hazel::Layer {
public:
	SandBoxLayer() : Layer("Sandbox layer") {
		auto cam = camera(5.0f, { 0.5f, 0.5f });
	}
	~SandBoxLayer() {}

	void OnEvent(Hazel::Event& event) override {
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
			if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
				HZ_CORE_INFO("Tab pressed");
				//HZ_INFO("{0}", event);
		
	}

	void OnImGuiRender() override {
		///*
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
		//*/
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
	
	
	sandBox->PushLayer(sbLayer);

	return sandBox;
}