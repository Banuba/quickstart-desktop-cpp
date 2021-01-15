#include "camera_renderer.hpp"
#include <bnb/utils/opengl.hpp>

#if (BOOST_OS_ANDROID || BOOST_OS_IOS)
#define BNB_GLSL_VERSION "#version 300 es\n"
#elif (BOOST_OS_WINDOWS || BOOST_OS_MACOS || BOOST_OS_LINUX)
#define BNB_GLSL_VERSION "#version 330 core \n"
#else
#error "Can't determine GLSL version for OS"
#endif

using namespace bnb;

camera_renderer::camera_renderer()
    : logged_object("camera_renderer")
    , m_cam_image_context(new renderer_gl_context())
{
    initialize_vertex_data();
    initialize_shader_data();
    glGenTextures(renderer_gl_context::TexturesAmount, m_cam_image_context->textures);
}

camera_renderer::~camera_renderer()
{
    release();
}

void camera_renderer::update_context(const frame_data &frame)
{
    update_camera_texture(frame);
}

void camera_renderer::draw()
{
    glUseProgram(m_cam_image_context->shader_program);

    for (int i = 0; i < renderer_gl_context::TexturesAmount; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_cam_image_context->textures[i]);
        glUniform1i(m_cam_image_context->texture_uniform_location[i], i);
    }
    glBindVertexArray(m_cam_image_context->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void camera_renderer::release()
{
    if (m_cam_image_context == nullptr)
        return;
    clear_gl_vertex_data(m_cam_image_context.get());
    glDeleteTextures(renderer_gl_context::TexturesAmount, m_cam_image_context->textures);
    glDeleteProgram(m_cam_image_context->shader_program);
    m_cam_image_context.release();
}

void camera_renderer::clear_gl_vertex_data(renderer_gl_context *context)
{
    if (context == nullptr)
        return;

    if (context->vao != 0) {
        glDeleteVertexArrays(1, &context->vao);
    }
    if (context->vbo != 0) {
        glDeleteBuffers(1, &context->vbo);
    }
    if (context->ebo != 0) {
        glDeleteBuffers(1, &context->ebo);
    }

    context->vao = 0;
    context->vbo = 0;
    context->ebo = 0;
}

void camera_renderer::initialize_vertex_data()
{
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float vertices[] = {
        // positions        // texture coords
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int vbo, vao, ebo;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    clear_gl_vertex_data(m_cam_image_context.get());

    m_cam_image_context->vao = vao;
    m_cam_image_context->vbo = vbo;
    m_cam_image_context->ebo = ebo;
}

void camera_renderer::initialize_shader_data()
{
    const char *vShader = BNB_GLSL_VERSION
                          " layout (location = 0) in vec3 aPos; \n"
                          " layout (location = 1) in vec2 aTexCoord; \n"
                          " out vec2 TexCoord; \n"
                          " void main() \n"
                          " { \n"
                              " gl_Position = vec4(aPos, 1.0); \n"
                              " TexCoord = aTexCoord; \n"
                          " } \n";

    const char *fShader = BNB_GLSL_VERSION
                          "out vec4 FragColor; \n"
                          "in vec2 TexCoord; \n"
                          "uniform sampler2D yTexture; \n"
                          "uniform sampler2D uTexture; \n"
                          "uniform sampler2D vTexture; \n"
                          "void main() \n"
                          "{ \n"
                              "float r, g, b, y, u, v; \n"
                              "float Umax = 0.436; \n"
                              "float Vmax = 0.615; \n"
                              "float Wr = 0.299; \n"
                              "float Wb = 0.114; \n"
                              "float Wg = 1. - Wr - Wb; \n"
                              "y = texture(yTexture, TexCoord).x; \n"
                              "u = texture(uTexture, TexCoord).x - 0.5; \n"
                              "v = texture(vTexture, TexCoord).x - 0.5; \n"
                              "r = y + v * ((1. - Wr) / Vmax); \n"
                              "g = y - u * ((Wb * (1. - Wb)) / (Umax * Wg)) - v * ((Wr * (1. - Wr)) / (Vmax * Wg)); \n"
                              "b = y + u * ((1. - Wb)/Umax); \n"
                              "FragColor = vec4(r, g, b, 1.0); \n"
                          "} \n";

    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShader, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        BOOST_LOG_SEV(m_logger, error) << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::exception();
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShader, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        BOOST_LOG_SEV(m_logger, error) << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        throw std::exception();
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        BOOST_LOG_SEV(m_logger, error) << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        throw std::exception();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_cam_image_context->texture_uniform_location[SamplerIndex::Y] = glGetUniformLocation(shaderProgram, SamplerName::Y);
    m_cam_image_context->texture_uniform_location[SamplerIndex::U] = glGetUniformLocation(shaderProgram, SamplerName::U);
    m_cam_image_context->texture_uniform_location[SamplerIndex::V] = glGetUniformLocation(shaderProgram, SamplerName::V);

    if (m_cam_image_context->shader_program != 0) {
        glDeleteProgram(m_cam_image_context->shader_program);
    }
    m_cam_image_context->shader_program = shaderProgram;
}

void camera_renderer::update_camera_texture(const frame_data &frame)
{
    for (int i = 0; i < renderer_gl_context::TexturesAmount; i++) {
        glBindTexture(GL_TEXTURE_2D, m_cam_image_context->textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const auto * texData = frame.m_full_yuv_image.m_y_plane.data();
        if (i == SamplerIndex::U) {
            texData = frame.m_full_yuv_image.m_u_plane.data();
        } else if (i == SamplerIndex::V) {
            texData = frame.m_full_yuv_image.m_v_plane.data();
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                     frame.m_full_yuv_image.m_format.m_width, frame.m_full_yuv_image.m_format.m_height, 0,
                     GL_RED, GL_UNSIGNED_BYTE, texData);
    }
}
