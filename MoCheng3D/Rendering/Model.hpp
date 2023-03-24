#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <string>
#include <vulkan/vulkan.hpp>

namespace MoCheng3D {

class Buffer;
class Texture;
class Model {

public:
    Model(std::string_view model_path, std::string_view texture_path);
    ~Model() = default;
    [[nodiscard("missing attr")]] auto& Get_Attr() { return vertex_attrs; }
    [[nodiscard("missing binding")]] auto& Get_Binding()
    {
        return vertex_bindings;
    }
    [[nodiscard("missing m_matrix")]] auto& Get_m_matrix() { return m_matrix; }
    [[nodiscard("missing index_buffer")]] auto Get_index_buffer()
    {
        return m_index_buffer;
    }
    [[nodiscard("Missing Texture")]] auto get_texture() { return texture; }
    // [[nodiscard("missing uv_buffer")]] auto Get_uv_buffer()
    // {
    //     return m_uv_buffer;
    // }
    // [[nodiscard("missing position_buffer")]] auto Get_position_buffer() { return m_position_buffer; }

    [[nodiscard]] auto Get_vertex_buffer()
    {
        if (vertex_buffers.empty()) {
            vertex_buffers.push_back(m_position_buffer);
            vertex_buffers.push_back(m_uv_buffer);
        }
        return vertex_buffers;
    }
    [[nodiscard]] auto Get_index() { return index_size; }
    void Update();

private:
    void Load_Model(std::string_view model_path);
    void Load_texture(std::string_view texture_path);
    std::shared_ptr<Texture> texture;
    std::shared_ptr<Buffer> m_position_buffer;

    std::shared_ptr<Buffer> m_uv_buffer;
    std::shared_ptr<Buffer> m_index_buffer;
    std::vector<vk::VertexInputAttributeDescription> vertex_attrs;
    std::vector<vk::VertexInputBindingDescription> vertex_bindings;
    uint32_t index_size;

    glm::mat4 m_matrix
        // { glm::mat4(1.0f) };

        { glm::rotate((glm::rotate(glm::mat4(1.0f), glm::radians(90.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f))),
            glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)) };
    std::vector<std::shared_ptr<Buffer>> vertex_buffers;
    float mAngle;
};

}