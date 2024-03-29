#include <bnb/glsl.frag>

BNB_DECLARE_SAMPLER_2D(0, 1, s_downscale_tex);

BNB_IN(0)
vec2 var_uv;

void main()
{
    float pixel = textureLod(BNB_SAMPLER_2D(s_downscale_tex), var_uv, 0.).x;

    float pp = textureLodOffset(BNB_SAMPLER_2D(s_downscale_tex), var_uv, 0., ivec2(1, 1)).x;
    float mp = textureLodOffset(BNB_SAMPLER_2D(s_downscale_tex), var_uv, 0., ivec2(-1, 1)).x;
    float mm = textureLodOffset(BNB_SAMPLER_2D(s_downscale_tex), var_uv, 0., ivec2(-1, -1)).x;
    float pm = textureLodOffset(BNB_SAMPLER_2D(s_downscale_tex), var_uv, 0., ivec2(1, -1)).x;

    float sum = (0.5*pixel + 0.125*(pp + mp + mm + pm));
    bnb_FragColor = vec4(sum, 0., 0., 0.);
}