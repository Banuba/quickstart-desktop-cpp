
#pragma once

#include <bnb/types/base_types.hpp>

#include "opengl.hpp"
#include <algorithm>

#define BNB_GLSL_VERSION "#version 330 core \n"

namespace bnb {

    class frame_surface_handler
    {
        uint32_t m_orientation = 0;
        bool m_mirrored = false;
        static const auto v_size = static_cast<uint32_t>(camera_orientation::deg_270) + 1;
        unsigned int m_vao = 0;
        unsigned int m_vbo = 0;
        unsigned int m_ebo = 0;
    public:
        static const float vertices[v_size][5*4];
        static const float vertices_mirrored[v_size][5*4];

        explicit frame_surface_handler(camera_orientation orientation)
            : m_orientation(static_cast<uint32_t>(orientation))
        {
            glGenVertexArrays(1, &m_vao);
            glGenBuffers(1, &m_vbo);
            glGenBuffers(1, &m_ebo);

            glBindVertexArray(m_vao);

            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[m_orientation]), vertices[m_orientation], GL_STATIC_DRAW);

            unsigned int indices[] = {
                    0, 1, 3, // first triangle
                    1, 2, 3  // second triangle
            };
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        virtual ~frame_surface_handler() final
        {
            if (m_vao != 0)
                glDeleteVertexArrays(1, &m_vao);

            if (m_vbo != 0)
                glDeleteBuffers(1, &m_vbo);

            if (m_ebo != 0)
                glDeleteBuffers(1, &m_ebo);

            m_vao = 0;
            m_vbo = 0;
            m_ebo = 0;
        }

        frame_surface_handler(const frame_surface_handler&) = delete;
        frame_surface_handler(frame_surface_handler&&) = delete;
        frame_surface_handler& operator= (const frame_surface_handler&) = delete;
        frame_surface_handler& operator= (frame_surface_handler&&) = delete;

        void set_orientation(camera_orientation orientation, bool mirrored)
        {
            if (m_orientation != static_cast<uint32_t>(orientation) ||
                m_mirrored != mirrored) {
                m_orientation = static_cast<uint32_t>(orientation);
                m_mirrored = mirrored;
                auto size = m_mirrored ? sizeof(vertices_mirrored[m_orientation]) : sizeof(vertices[m_orientation]);
                auto *data = m_mirrored ? vertices_mirrored[m_orientation] : vertices[m_orientation];
                glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
                glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
            }
        }

        void draw() {
            glBindVertexArray(m_vao);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);
        }
    };
}
