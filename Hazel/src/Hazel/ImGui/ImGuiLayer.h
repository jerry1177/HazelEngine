#pragma once
//#include "Hazel/Core.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel {
	class HAZEL_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();
		
		void Begin();
		void OnAttach() override;
		void OnDetach() override;
		void OnEvent(Event& e) override;
		void End();

		void SetBlockEvents(bool block) { m_BlockEvents = block; }
	private:
		float m_Time = 0.0f;
		bool m_BlockEvents = true;
	};
}

