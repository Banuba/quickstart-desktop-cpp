// #include <bnb/renderer/renderer.hpp>
// #include <bnb/utils/opengl.hpp>

// #include <meshfx.hpp>
// #include <bnb/utils/resources/resource_manager.hpp>
// #include <bnb/utils/profiler.hpp>

// namespace
// {
//     const char* vertex_shader = \
//         " precision highp float; \n "
//         " layout (location = 0) in vec3 aPos; \n"
//         " layout (location = 1) in vec2 aTexCoord; \n"
//         " out vec2 TexCoord; \n"
//         " void main() \n"
//         " { \n"
//             " gl_Position = vec4(aPos, 1.0); \n"
//             " TexCoord = aTexCoord; \n"
//         " } \n";

//     const char* fragment_shader = \
//         "precision highp float; \n "
//         "out vec4 FragColor; \n"
//         "in vec2 TexCoord; \n"
//         "uniform sampler2D yTexture; \n"
//         "uniform sampler2D uvTexture; \n"

//         "void main() \n"
//         "{ \n"
//             "float r, g, b, y, u, v; \n"
//             "float Umax = 0.436; \n"
//             "float Vmax = 0.615; \n"
//             "float Wr = 0.299; \n"
//             "float Wb = 0.114; \n"
//             "float Wg = 1. - Wr - Wb; \n"
//             "y = texture(yTexture, TexCoord).x; \n"
//             "u = texture(uvTexture, TexCoord).x - 0.5; \n"
//             "v = texture(uvTexture, TexCoord).y - 0.5; \n"

//             "r = y + v * ((1. - Wr) / Vmax); \n"
//             "g = y - u * ((Wb * (1. - Wb)) / (Umax * Wg)) - v * ((Wr * (1. - Wr)) / (Vmax * Wg)); \n"
//             "b = y + u * ((1. - Wb)/Umax); \n"
//             "FragColor = vec4(r, g, b, 1.0); \n"
//         "} \n";
// }

// using namespace bnb;

// using gl_context = renderer_gl_context;

// renderer::renderer()
//     : logged_object("Renderer")
//     , m_current_frame_number(0)
//     , m_camera_program("RendererCamera", vertex_shader, fragment_shader)
//     , m_face_mesh()
//     , m_is_meshfx_initialized(false)
// {
//     GL_CALL(glGenTextures(gl_context::TexturesAmount, m_gl_context.textures));
// }

// renderer::~renderer()
// {
//     if(m_is_meshfx_initialized) {
//         meshfx_destroy();
//     }
// }

// void renderer::surface_changed(int32_t width, int32_t height, float pixel_density_w, float pixel_density_h)
// {
//     GL_CALL(glViewport(0, 0, width, height));
//     _width = width;
//     _height = height;
//     m_imgui_handler.set_display_size(width, height, pixel_density_w, pixel_density_h);
// }

// void renderer::draw(const frame_data& frame_data)
// {
//     BNB_PRF_BLOCK("RendererDraw");
//     ImGui::NewFrame();
//     update_camera_texture(frame_data);

//     if (m_video_texture != nullptr)
//     {
//         meshfx_decoded_video_frame(m_video_texture->get_gl_texture());
//     }

//     if( frame_data.m_background )
//     {
//         auto const & bg = frame_data.m_background.get();
//         meshfx_bg_mask_texture( bg.m_back.data(), bg.m_width, bg.m_height );
//     }

//     GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
//     GL_CALL(glEnable(GL_BLEND));
//     GL_CALL(glClearColor(0.0,0.0,0.0,0.0));
//     GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

//     m_camera_program.use();

//     m_gl_context.texture_uniform_location[SamplerIndex::Y] = glGetUniformLocation(m_camera_program.handle(), "yTexture");
//     m_gl_context.texture_uniform_location[SamplerIndex::UV] = glGetUniformLocation(m_camera_program.handle(), "uvTexture");

//     for (auto i = 0u; i < gl_context::TexturesAmount; i++) {
//         GL_CALL(glActiveTexture(GL_TEXTURE0 + i));
//         GL_CALL(glBindTexture(GL_TEXTURE_2D, m_gl_context.textures[i]));
//         GL_CALL(glUniform1i(m_gl_context.texture_uniform_location[i], i));
//     }

//     m_gl_context.m_frame_surface.draw();

//     m_camera_program.unuse();

//     if (frame_data.m_background) {
//         BNB_PRF_BLOCK("BackgroundDraw");
//         m_background_renderer.draw(frame_data, 1);
//     }

//     m_body_pose_renderer.draw(frame_data, 1);
//     m_shoulders_pose_renderer.draw(frame_data, 1);

//     // TODO(a_cherkes): инкапсулировать FRX в recognizer-е
//     auto& frx_rec_res = frame_data.m_frx_recognition_result;
//     if(frx_rec_res && frx_rec_res.get().m_face_rectangle.hasFaceRectangle == 1)
//     {
//         BNB_PRF_BLOCK("FRXBasedDataDraw");
//         const float* model_view = frx_rec_res.get().m_camera_position.model_view_m;
//         const float* projection = frx_rec_res.get().m_camera_position.projection_m;
//         const vertices_t& vertices = frx_rec_res.get().m_vertices;

//         if (m_is_meshfx_initialized)
//         {
//             meshfx_update(projection, model_view, vertices.data(), nullptr, 0);
//         }
//         else
//         {
//             auto frame_width = frame_data.m_full_yuv_image.m_format.m_width;
//             auto frame_height = frame_data.m_full_yuv_image.m_format.m_height;
//             const auto orientation = frame_data.m_full_yuv_image.m_format.m_orientation;
//             if (orientation == camera_orientation::CAMERA_ORIENTATION_90 || orientation == camera_orientation::CAMERA_ORIENTATION_270) {
//                 std::swap(frame_width, frame_height);
//             }
//             const float scale_x_coef = _height / (float)frame_height *
//                                frame_width / (float)_width;

// #if !OPEN_FACE_BUILD
//             m_face_mesh_renderer.draw(frame_data, scale_x_coef);
// #endif
//             m_landmarks_renderer.draw(frame_data, scale_x_coef);
//             if (frame_data.m_eyes)
//             {
//                 m_eyes_renderer.draw(frame_data, 1);
//                 if (frame_data.m_sight_direction_data)
//                 {
//                     m_sight_direction_renderer.draw(frame_data, 1);
//                 }
//             }
//             m_face_rect_renderer.draw(frame_data, 1);
//         }
//     }

//     BNB_PRF_BLOCK("IMGUIDraw");

//     //m_imgui_renderer.draw(frame_data);
//     ImGui::Render();
// }

// void renderer::meshfx_init(const std::string& effect_url)
// {
//     meshfx_preload(effect_url.c_str());

// #if !BOOST_OS_IOS
//     const char* error = meshfx_activate(effect_url.c_str(), m_face_mesh.pos_vb(), m_face_mesh.uv_vb());
// #else
//     const char* error = meshfx_activate(effect_url.c_str(), bnb::resource_manager::instance().get_absolute_url("glfx/head.verts").path().c_str(), bnb::resource_manager::instance().get_absolute_url("glfx/head1.tex").path().c_str());
// #endif

//     if(error != nullptr) {
//         BOOST_LOG_SEV(m_logger, severity_level::error) << "Error in meshfx_init: " << error;
//         throw std::runtime_error(std::string(error));
//     }

//     m_is_meshfx_initialized = true;
// }

// void renderer::meshfx_destroy()
// {
//     ::meshfx_destroy();
//     m_is_meshfx_initialized = false;
// }

// void renderer::meshfx_msg_js(char const* command, int instance_id, int index, char const* str_param)
// {
//     ::meshfx_msg_js(command, instance_id, index, str_param);
// }

// void renderer::meshfx_reset()
// {
//     ::meshfx_reset();
// }

// void renderer::update_camera_texture(const frame_data &frame)
// {
//     m_gl_context.m_frame_surface.set_orientation(frame.m_full_yuv_image.m_format.m_orientation,
//         frame.m_full_yuv_image.m_format.m_is_mirrored);

//     GL_CALL(glBindTexture(GL_TEXTURE_2D, m_gl_context.textures[SamplerIndex::Y]));
//     GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//     GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    
//     const auto y_internal_format = BOOST_OS_IOS ? GL_R8 : GL_RG;
//     GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, y_internal_format
//                  ,frame.m_full_yuv_image.m_format.m_width, frame.m_full_yuv_image.m_format.m_height, 0,
//                  GL_RED, GL_UNSIGNED_BYTE, frame.m_full_yuv_image.m_y_plane.data()));

//     GL_CALL(glBindTexture(GL_TEXTURE_2D, m_gl_context.textures[SamplerIndex::UV]));
//     GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
//     GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    
//     const auto uv_internal_format = BOOST_OS_IOS ? GL_RG8 : GL_RG;
//     GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, uv_internal_format
//                      ,frame.m_full_yuv_image.m_format.m_width/2, frame.m_full_yuv_image.m_format.m_height/2, 0,
//                  GL_RG, GL_UNSIGNED_BYTE, frame.m_full_yuv_image.m_uv_plane.data()));

//     GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
// }


// void renderer::set_video_filename(const std::string& filename)
// {
//     m_video_texture = nullptr;
//     if (!filename.empty())
//     {
//         m_video_texture = std::make_unique<video_texture>(filename);
//     }
// }


// void renderer::play_video(bool is_loop, double speed)
// {
//     (void)is_loop; //UNUSED
//     (void)speed; //UNUSED

//     BNB_WRITE_LOG_MESSAGE(warning) << "play video: speed and lopping doesn't support";

//     if (m_video_texture != nullptr)
//     {
//         m_video_texture->play();
//     }
// }


// void renderer::play_video_range(bool is_loop, double start_time, double stop_time, double speed)
// {
//     (void)is_loop; //UNUSED
//     (void)start_time; //UNUSED
//     (void)stop_time; //UNUSED
//     (void)speed; //UNUSED

//     BNB_WRITE_LOG_MESSAGE(warning) << "play video range: doesnt support. Just play all video";

//     if (m_video_texture != nullptr)
//     {
//         m_video_texture->play();
//     }
// }


// void renderer::pause_video()
// {
//     if (m_video_texture != nullptr)
//     {
//         m_video_texture->pause();
//     }
// }


// void renderer::stop_video()
// {

//     if (m_video_texture != nullptr)
//     {
//         m_video_texture->stop();
//     }
// }
