#include <bnb/glsl.vert>

layout(location = 0) in vec3 attrib_pos;

BNB_DECLARE_SAMPLER_2D(0, 1, s_upscale_tex);

BNB_OUT(0)
vec4 var_uv;

void main()
{
    vec2 v = attrib_pos.xy;
    gl_Position = vec4(v, 0., 1.);
    var_uv.xy = v * 0.5 + 0.5;
    var_uv.zw = var_uv.xy + 0.5 / vec2(textureSize(BNB_SAMPLER_2D(s_upscale_tex), 0));
}