#pragma once
#include "entt.hpp"
#include "Hazel/Core/TimeStep.h"
namespace Hazel {
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "");

		entt::registry& Reg() { return m_Registry; }
		void SetViewportSize(uint32_t width, uint32_t height);

		void OnUpdate(TimeStep ts);

	private:
		uint32_t m_ViewportWidth = 0;
		uint32_t m_ViewportHeight = 0;
		entt::registry m_Registry;
		friend class Entity;
	};

	

}