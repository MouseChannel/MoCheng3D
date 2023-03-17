#include "MoCheng3D/Helper/Camera.hpp"

namespace MoCheng3D {

Camera::Camera()
{

    m_position = glm::vec3(0.0f, -1.0f, -7.0f);
    m_front = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    m_up = glm::vec3(1.0f, 0.0f, 0.0f);
    m_pMatrx = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 66.0f);
    m_vMatrix = glm::mat4(1.0f);
    Update();
}
void Camera::Update()
{
    m_vMatrix = glm::lookAt(m_position, m_position + m_front, m_up);
}
}