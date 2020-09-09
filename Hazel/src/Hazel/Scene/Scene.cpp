#include "hzpch.h"
#include "Hazel/Renderer/Renderer2D.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Component.h"
#include "Hazel/Scene/Entity.h"
#include "glm/glm.hpp"

namespace Hazel {

	Scene::Scene()
	{
		
	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view) {
			CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio) {
				cameraComponent.Camera.SetViewportSize(width, height);
			}
			
		}
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
			// move to scene on scene play
			if (!nsc.Instance) {
				nsc.Instance = nsc.InstantiateScript();
				nsc.Instance->m_Entity = Entity{ entity, this };
				nsc.Instance->OnCreate();

			}
			nsc.Instance->OnUpdate(ts);

			});
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTranform = nullptr;

		auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
		for (auto entity : group) {
			auto [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);
			if (camera.Primary) {
				mainCamera = &camera.Camera;
				cameraTranform = &transform.Transform;
				break;
			}
		}
		// Render 2D
		if (mainCamera) {
			Renderer2D::BeginScene(*mainCamera, *cameraTranform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group) {
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);

			}
			Renderer2D::EndScene();
		}
	}

}