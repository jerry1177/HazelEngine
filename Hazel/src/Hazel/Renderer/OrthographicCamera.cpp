#include "hzpch.h"
#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Hazel {
	OrthogrphicCamera::OrthogrphicCamera(float left, float right, float bottom, float top)
	: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f) {
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	}

	void OrthogrphicCamera::RecalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * 
			glm::rotate(glm::mat4(1), glm::radians(m_Rotation), glm::vec3(0,0,1));

		m_ViewMatrix = glm::inverse(transform);
		RecalculateVPMatrix();
	}

	void OrthogrphicCamera::RecalculateVPMatrix() {
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}


}