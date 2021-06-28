#include "VertexShader.h"
#include "RenderSystem.h"
#include <exception>

VertexShader::VertexShader(const void* shader_byte_code, size_t byte_code_size, RenderSystem* rs) : render_system(rs)
{
    if(!SUCCEEDED(render_system->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vertex_shader)))
        throw std::exception("FAILED TO CREATE VERTEX SHADER");
}

VertexShader::~VertexShader()
{
    m_vertex_shader->Release();
}
