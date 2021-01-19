#include "camera_renderer.hpp"

#include "opengl.hpp"

#include "../../offscreen_effect_player/include/utils.hpp"

#include <opencv2/opencv.hpp>

#define BNB_GLSL_VERSION "#version 330 core \n"

namespace bnb {

camera_renderer::camera_renderer() :
    m_gl_context(new renderer_gl_context())
{
    initialize_vertex_data();
    initialize_shader_data();
    glGenTextures(renderer_gl_context::TexturesAmount, m_gl_context->textures);
}

camera_renderer::~camera_renderer()
{
    release();
}

void camera_renderer::update_context(std::shared_ptr<bnb::full_image_t> image)
{
    update_camera_texture(image);
}

int camera_renderer::draw()
{
    if (!m_need_draw) {
        return -1;
    }

    std::cout << "draw START" << std::endl;

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // m_camera_program.use();

    // for (auto i = 0u; i < renderer_gl_context::TexturesAmount; i++) {
    //     glActiveTexture(GL_TEXTURE0 + i);
    //     glBindTexture(GL_TEXTURE_2D, m_gl_context->textures[i]);
    //     glUniform1i(m_gl_context->texture_uniform_location[i], i);
    // }

    // m_gl_context.m_frame_surface.draw();



    glUseProgram(m_gl_context->m_frame_surface.shader_program);

    for (int i = 0; i < renderer_gl_context::TexturesAmount; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_gl_context->textures[i]);
        glUniform1i(m_gl_context->texture_uniform_location[i], i);
    }
    std::cout << "draw glBindVertexArray" << std::endl;
    glBindVertexArray(m_gl_context->m_frame_surface.vao);
    std::cout << "draw glDrawElements" << std::endl;

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

    std::cout << "draw FINISH" << std::endl;

    m_need_draw = false;
    return 1;
}

void camera_renderer::release()
{
    if (m_gl_context == nullptr)
        return;
    clear_gl_vertex_data(m_gl_context.get());
    glDeleteTextures(renderer_gl_context::TexturesAmount, m_gl_context->textures);
    glDeleteProgram(m_gl_context->m_frame_surface.shader_program);
    m_gl_context.release();
}

void camera_renderer::clear_gl_vertex_data(renderer_gl_context *context)
{
    if (context == nullptr)
        return;

    if (context->m_frame_surface.vao != 0) {
        glDeleteVertexArrays(1, &context->m_frame_surface.vao);
    }
    if (context->m_frame_surface.vbo != 0) {
        glDeleteBuffers(1, &context->m_frame_surface.vbo);
    }
    if (context->m_frame_surface.ebo != 0) {
        glDeleteBuffers(1, &context->m_frame_surface.ebo);
    }

    context->m_frame_surface.vao = 0;
    context->m_frame_surface.vbo = 0;
    context->m_frame_surface.ebo = 0;
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

    m_gl_context->m_frame_surface.vao = vao;
    m_gl_context->m_frame_surface.vbo = vbo;
    m_gl_context->m_frame_surface.ebo = ebo;
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
        throw std::exception();
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    m_gl_context->texture_uniform_location[SamplerIndex::Y] = glGetUniformLocation(shaderProgram, SamplerName::Y);
    m_gl_context->texture_uniform_location[SamplerIndex::UV] = glGetUniformLocation(shaderProgram, SamplerName::UV);
    // m_gl_context->texture_uniform_location[SamplerIndex::V] = glGetUniformLocation(shaderProgram, SamplerName::V);

    if (m_gl_context->m_frame_surface.shader_program != 0) {
        glDeleteProgram(m_gl_context->m_frame_surface.shader_program);
    }
    m_gl_context->m_frame_surface.shader_program = shaderProgram;
}

// void camera_renderer::update_camera_texture(const bnb::full_image_t& image)
// {
//     for (int i = 0; i < renderer_gl_context::TexturesAmount; i++) {
//         glBindTexture(GL_TEXTURE_2D, m_cam_image_context->textures[i]);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//         const auto * texData = image->m_full_yuv_image.m_y_plane.data();
//         if (i == SamplerIndex::U) {
//             texData = image->m_full_yuv_image.m_u_plane.data();
//         } else if (i == SamplerIndex::V) {
//             texData = image->m_full_yuv_image.m_v_plane.data();
//         }

//         glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
//                      image->m_full_yuv_image.m_format.m_width, image->m_full_yuv_image.m_format.m_height, 0,
//                      GL_RED, GL_UNSIGNED_BYTE, texData);
//     }
// }

void camera_renderer::update_camera_texture(std::shared_ptr<bnb::full_image_t> image)
{
    m_gl_context->m_frame_surface.m_orientation = image->get_format().orientation;

    if (image->has_data<bnb::yuv_image_t>()) {
        std::cout << "Get yuv" << std::endl;
    } else if (image->has_data<bnb::bpc8_image_t>()) {
        std::cout << "Get bpc8" << std::endl;
        // const auto& bpc8 = image->get_data<bnb::bpc8_image_t>();
    } else {
        std::cout << "Unsuported format of image from camera";
        return;
    }

    // const auto& yuv = image->get_data<bnb::yuv_image_t>();
    const auto& bpc8 = image->get_data<bnb::bpc8_image_t>();

    // cv::Mat cvmat(image->get_format().width, image->get_format().height, CV_8UC4, bpc8.get_data());
    // cv::imwrite("D:/MyImage.jpg", cvmat);

    glBindTexture(GL_TEXTURE_2D, m_gl_context->textures[SamplerIndex::Y]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    cur_y_plane.reset();
    cur_uv_plane.reset();
    cur_y_plane = convert_bpc8_to_y_plane(bpc8, image->get_format().width, image->get_format().height);
    cur_uv_plane = convert_bpc8_to_uv_plane(bpc8, image->get_format().width, image->get_format().height, false);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG,
                 image->get_format().width, image->get_format().height, 0,
                 GL_RED, GL_UNSIGNED_BYTE, cur_y_plane.get());

    glBindTexture(GL_TEXTURE_2D, m_gl_context->textures[SamplerIndex::UV]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG,
                 image->get_format().width / 2, image->get_format().height / 2, 0,
                 GL_RG, GL_UNSIGNED_BYTE, cur_uv_plane.get());

    glBindTexture(GL_TEXTURE_2D, 0);

    m_need_draw = true;
}

} // bnb