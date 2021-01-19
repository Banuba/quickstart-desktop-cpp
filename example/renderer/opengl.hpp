#pragma once

#include <glad/glad.h>
#define BNB_GL

#include <bnb/utils/singleton.hpp>

namespace bnb::gl
{
    enum class mali_gpu_family
    {
        generic, // all not listed
        bifrost,
    };

    enum class adreno_gpu_family
    {
        generic,
        _61x,
    };

    class context_info : public bnb::singleton<context_info>
    {
    public:
        struct caps_t
        {
            GLint max_texture_size{1024};
            bool has_rgba16f{false};
        } mutable caps;

        struct quirks_t
        {
            bool msaa_bug{false}; // Adreno (TM) 616, 615, 612 crashes on rendering into MSAA RG16F target
        } quirks;

        std::pair<int, int> gl_version{};

    public:
        context_info();
        virtual ~context_info() = default;

        void check_error(const char* file, int line);

    private:
        bool is_rgba16f_available();

        const char* error_code_to_string(GLenum error_code) const;
        void on_error(GLenum error_code, const char* file, int line);
    };

} // namespace bnb::gl

#define GL_CHECK_ERROR() bnb::gl::context_info::instance().check_error(__FILE__, __LINE__)
#define GL_CALL(FUNC) [&]() {FUNC; GL_CHECK_ERROR(); }()

#if BNB_OS_IOS && BNB_GL_MARKERS_ENABLED
    #define BNB_GL_INIT() ((void) 0)
    #define BNB_GL_START_GROUP(name) glPushGroupMarkerEXT(0, name)
    #define BNB_GL_END_GROUP() glPopGroupMarkerEXT()
// Macro for naming resources
    #define BNB_GL_LABEL(obj, name) glLabelObjectEXT(resource_type(obj), obj, 0, name)
#elif BNB_OS_WINDOWS && BNB_GL_MARKERS_ENABLED
namespace bnb::gl
{
    void enable_debug_log();
}
    #define BNB_GL_INIT() bnb::gl::enable_debug_log()
    #define BNB_GL_START_GROUP(name) glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 1, -1, name)
    #define BNB_GL_END_GROUP() glPopDebugGroup()
    #define BNB_GL_LABEL(obj, name) glObjectLabel(resource_type(obj), obj, -1, name)
#elif BNB_OS_ANDROID && BNB_GL_MARKERS_ENABLED
// clang-format off
extern PFNGLDEBUGMESSAGECONTROLKHRPROC  glDebugMessageControlKHR;
extern PFNGLDEBUGMESSAGEINSERTKHRPROC   glDebugMessageInsertKHR;
extern PFNGLDEBUGMESSAGECALLBACKKHRPROC glDebugMessageCallbackKHR;
extern PFNGLGETDEBUGMESSAGELOGKHRPROC   glGetDebugMessageLogKHR;
extern PFNGLPUSHDEBUGGROUPKHRPROC       glPushDebugGroupKHR;
extern PFNGLPOPDEBUGGROUPKHRPROC        glPopDebugGroupKHR;
extern PFNGLOBJECTLABELKHRPROC          glObjectLabelKHR;
extern PFNGLGETOBJECTLABELKHRPROC       glGetObjectLabelKHR;
extern PFNGLOBJECTPTRLABELKHRPROC       glObjectPtrLabelKHR;
extern PFNGLGETOBJECTPTRLABELKHRPROC    glGetObjectPtrLabelKHR;
extern PFNGLGETPOINTERVKHRPROC          glGetPointervKHR;
// clang-format on
namespace bnb::gl
{
    void load_khr_debug();
    void enable_debug_log();
} // namespace bnb::gl

    #define BNB_GL_INIT() bnb::gl::enable_debug_log()

    #define BNB_GL_START_GROUP(name) \
        if (!glPushDebugGroupKHR)    \
            ;                        \
        else                         \
            glPushDebugGroupKHR(GL_DEBUG_SOURCE_APPLICATION_KHR, 1, -1, name)

    #define BNB_GL_END_GROUP()   \
        if (!glPopDebugGroupKHR) \
            ;                    \
        else                     \
            glPopDebugGroupKHR()

    #define BNB_GL_LABEL(obj, name) \
        if (!glObjectLabelKHR)      \
            ;                       \
        else                        \
            glObjectLabelKHR(resource_type(obj), obj, -1, name)

#else

    #define BNB_GL_INIT() ((void) 0)
    #define BNB_GL_START_GROUP(name) ((void) 0)
    #define BNB_GL_END_GROUP() ((void) 0)
    #define BNB_GL_LABEL(obj, name) ((void) 0)

#endif

#if BNB_GL_MARKERS_ENABLED
namespace bnb::gl
{
    struct scope_marker
    {
        scope_marker(char const* name)
        {
            BNB_GL_START_GROUP(name);
        }
        ~scope_marker()
        {
            BNB_GL_END_GROUP();
        }
    };
} // namespace bnb::gl

    // Macro for marking code scope in capture gl frame output
    #define BNB_GL_SCOPE_CONCAT1(a, b) a##b
    #define BNB_GL_SCOPE_CONCAT(a, b) BNB_GL_SCOPE_CONCAT1(a, b)
    #define BNB_GL_SCOPE(name)                                        \
        bnb::gl::scope_marker BNB_GL_SCOPE_CONCAT(gl_scope, __LINE__) \
        {                                                             \
            name                                                      \
        }
#else
    #define BNB_GL_SCOPE(name) ((void) 0)
#endif

#ifdef BNB_INTERCEPT_GL
    #include <bnb/utils/internal/opengl_intercept.hpp>
#endif
