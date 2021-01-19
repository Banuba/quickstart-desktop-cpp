// #pragma once

// #include <bnb/utils/log/logged_object.hpp>

// #include <cstdint>
// #include <vector>
// #include <queue>

// #include "renderer_gl_context.hpp"

// #include <bnb/renderer/frame_data/landmarks_renderer.hpp>
// #include <bnb/renderer/frame_data/face_mesh_renderer.hpp>
// #include <bnb/renderer/frame_data/face_rect_renderer.hpp>
// #include <bnb/renderer/frame_data/eyes_renderer.hpp>
// #include <bnb/renderer/frame_data/body_pose_renderer.hpp>
// #include <bnb/renderer/frame_data/shoulders_pose_renderer.hpp>
// #include <bnb/renderer/frame_data/sight_direction_renderer.hpp>
// #include <bnb/renderer/frame_data/hair_skin_colors_renderer.hpp>
// #include <bnb/renderer/frame_data/background_renderer.hpp>
// #include <bnb/renderer/frame_data/frame_data_debug_hud_renderer.hpp>
// #include <bnb/renderer/imgui_handler.hpp>
// #include <bnb/renderer/face_mesh.hpp>
// #include <bnb/renderer/video_texture.hpp>

// namespace bnb
// {
//     class renderer : protected logged_object
//     {
//     public:
//         renderer();
//         ~renderer() override;

//         void surface_changed(int32_t width, int32_t height, float pixel_density_w, float pixel_density_h);
//         void draw(const frame_data& frame_data);

//         void set_video_filename(const std::string& filename);
//         void play_video(bool is_loop, double speed);
//         void play_video_range(bool is_loop, double start_time, double stop_time, double speed);
//         void pause_video();
//         void stop_video();

//         void meshfx_init(const std::string& effect_url);
//         void meshfx_destroy();
//         void meshfx_msg_js(char const * command, int instance_id, int index, char const * str_param);
//         void meshfx_reset();

//     private:
//         void update_camera_texture(const frame_data& frame);

//         static void clear_gl_vertex_data(renderer_gl_context *context);

//     private:
//         uint64_t    m_current_frame_number;

//         renderer_gl_context m_gl_context;
//         program m_camera_program;

//         // TODO(a_cherkes): вернуть динамический полиморфизм для отладочных визуализаторов, когда
//         // frame_data будет быстро копироваться через swap.
//         background_renderer m_background_renderer;
//         landmarks_renderer m_landmarks_renderer;
//         eyes_renderer m_eyes_renderer;
//         face_mesh_renderer m_face_mesh_renderer;
//         face_rect_renderer m_face_rect_renderer;
//         body_pose_renderer m_body_pose_renderer;
//         shoulders_pose_renderer m_shoulders_pose_renderer;
//         sight_direction_renderer m_sight_direction_renderer;
//         frame_data_debug_hud_renderer m_imgui_renderer;

//         imgui_handler m_imgui_handler;
//         face_mesh m_face_mesh;

//         int32_t _width;
//         int32_t _height;

//         bool m_is_meshfx_initialized;

//         std::unique_ptr<video_texture> m_video_texture;
//     };
// }
