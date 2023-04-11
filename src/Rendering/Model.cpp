#include "Rendering/Model.hpp"
#include "Rendering/Texture.hpp"
#define TINYOBJLOADER_IMPLEMENTATION
#include "Tool/tiny_obj_loader.hpp"
#include "Wrapper/Buffer.hpp"
namespace MoCheng3D {

Model::Model(std::string_view model_path, std::string_view texture_path)
{
    Load_Model(model_path);
    Load_texture(texture_path);
}
void Model::Load_Model(std::string_view model_path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    std::string warn;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.data())) {
        throw std::runtime_error("Error: failed to load model");
    }
    std::vector<float> m_positions;
    std::vector<float> m_uv;
    std::vector<uint32_t> m_index;
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {

            m_positions.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            m_positions.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            m_positions.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            m_uv.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
            m_uv.push_back(1.0f - attrib.texcoords[2 * index.texcoord_index + 1]);

            m_index.push_back(m_index.size());
        }
    }

    m_position_buffer = Buffer::CreateDeviceBuffer(
        m_positions.data(), m_positions.size() * sizeof(m_positions[0]), vk::BufferUsageFlagBits::eVertexBuffer);
    m_uv_buffer = Buffer::CreateDeviceBuffer(m_uv.data(), m_uv.size() * sizeof(m_uv[0]), vk::BufferUsageFlagBits::eVertexBuffer);
    m_index_buffer = Buffer::CreateDeviceBuffer(
        m_index.data(), m_index.size() * sizeof(m_index[0]), vk::BufferUsageFlagBits::eIndexBuffer);

    std::cout << "model index size: " << m_index.size() << std::endl;
    index_size = m_index.size();
    // make vertex binding
    vk::VertexInputBindingDescription vertex_position_binding;
    vertex_position_binding.setBinding(0).setInputRate(vk::VertexInputRate::eVertex).setStride(sizeof(float) * 3);

    vk::VertexInputBindingDescription vertex_uv_binding;
    vertex_uv_binding.setBinding(1).setInputRate(vk::VertexInputRate::eVertex).setStride(sizeof(float) * 2);
    vertex_bindings.push_back(vertex_position_binding);

    vertex_bindings.push_back(vertex_uv_binding);
    // make vertex attribution
    vk::VertexInputAttributeDescription vertex_position_attr;
    vertex_position_attr.setBinding(0).setLocation(0).setFormat(vk::Format::eR32G32B32Sfloat).setOffset(0);
    vk::VertexInputAttributeDescription vertex_uv_attr;
    vertex_uv_attr.setBinding(1).setLocation(2).setFormat(vk::Format::eR32G32Sfloat).setOffset(0);

    vertex_attrs.push_back(vertex_position_attr);

    vertex_attrs.push_back(vertex_uv_attr);
}
void Model::Load_texture(std::string_view texture_path)
{
    texture.reset(new Texture(texture_path));
}
void Model::Update()
{
    m_matrix = glm::rotate(m_matrix, glm::radians(mAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    mAngle = 0.99f;
}
} // namespace MoCheng3D