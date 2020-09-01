#include "hzpch.h"
#include "OrthoCameraController.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/Input.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) : m_AspectRatio(aspectRatio),
		m_Bounds({-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel}),
		m_Camera(m_Bounds.Left,  m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		HZ_PROFILE_FUNCTION();
		
		if (Input::IsKeyPressed(Key::A))
			m_Position.x -= m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::W))
			m_Position.y += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::D))
			m_Position.x += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::S))
			m_Position.y -= m_CameraTranslationSpeed * ts;


		if (m_Rotation) {
			
			if (Input::IsKeyPressed(Key::Left)) {
				m_CameraRotation -= m_CameraRotationSpeed * ts;
				HZ_CORE_INFO("HI");
			}
			if (Input::IsKeyPressed(Key::Right)) {
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

	void OrthographicCameraController::Resize(float width, float height)
	{
		m_AspectRatio = width / height;
		CalculateView();
	}

	void OrthographicCameraController::CalculateView() {
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjectionMatrix(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset()*0.5f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		Resize((float)e.GetWidth(), (float)e.GetHeight()); 
		return false;
	}

}