#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace MoCheng3D {

class Camera {
public:
    Camera();
    [[nodiscard("missing v_matrix")]] auto& Get_v_matrix() { return m_vMatrix; }
    [[nodiscard("missing p_matrix")]] auto& Get_p_matrix() { return m_pMatrx; }
    void Update();

private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::mat4 m_vMatrix;
    glm::mat4 m_pMatrx;
};

}