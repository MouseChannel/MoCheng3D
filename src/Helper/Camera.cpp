#include "MoCheng3D/Helper/Camera.hpp"

namespace MoCheng3D {

Camera::Camera()
{
    m_position = glm::vec3(0.0f, 0.0f, 800.0f);
    m_front = glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f));
    m_up = glm::vec3(1.0f, 0.0f, 0.0f);
    m_pMatrx = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    m_vMatrix = glm::mat4(1.0f);
}

}