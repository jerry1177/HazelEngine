#pragma once
#include "Hazel/Scene/Entity.h"
#include "Hazel/Core/TimeStep.h"
namespace Hazel {

	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}
		template<typename T>
		T& GetComponent() {
			return m_Entity.GetComponent<T>();
		}
		template <typename T>
		bool HasComponent() {
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}
		template<typename T>
		void RemoveComponent() {
			HZ_CORE_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

	protected:

		virtual void OnCreate() {}
		virtual void OnUpdate(TimeStep ts) {}
		virtual void OnDestroy() {}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}