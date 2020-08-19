#include "hzpch.h"
#include "OrthoCameraController.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/Input.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) : m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		HZ_PROFILE_FUNCTION();
		
		if (Input::IsKeyPressed(HZ_KEY_A))
			m_Position.x -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_W))
			m_Position.y += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_D))
			m_Position.x += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_S))
			m_Position.y -= m_CameraTranslationSpeed * ts;


		if (m_Rotation) {
			
			if (Input::IsKeyPressed(HZ_KEY_LEFT)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
				HZ_CORE_INFO("HI");
			}
			if (Input::IsKeyPressed(HZ_KEY_RIGHT)) {
				m_CameraRotation += m_CameraRotationSpeed * ts;
				
			}
			m_Camera.SetRotaion(m_CameraRotation);
		}
		m_Camera.SetPosition(m_Position);
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolledEvent));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResizeEvent));
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset()*0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}