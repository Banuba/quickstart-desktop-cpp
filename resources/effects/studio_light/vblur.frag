#include <bnb/glsl.frag>

BNB_IN(0)
vec3 var_uv;

// kernel: 0.028532	0.067234	0.124009	0.179044	0.20236	0.179044	0.124009	0.067234	0.028532
const float s0 = 0.20236;
const float s1 = 0.124009 + 0.179044;
const float s2 = 0.028532 + 0.067234;

BNB_DECLARE_SAMPLER_2D(0, 1, tex_src);

void main()
{
    int d = int(var_uv.z);

    float inv_sz = 1. / float(textureSize(BNB_SAMPLER_2D(tex_src), 0)[d]);

    float o1 = inv_sz * (1. + 0.179044 / s1);
    float o2 = inv_sz * (3. + 0.067234 / s2);

    vec2 uv1p = var_uv.xy;
    vec2 uv1m = var_uv.xy;

    vec2 uv2p = var_uv.xy;
    vec2 uv2m = var_uv.xy;

    if (d == 0) {
        uv1p[0] += o1;
        uv1m[0] -= o1;
        uv2p[0] += o2;
        uv2m[0] -= o2;
    } else {
        uv1p[1] += o1;
        uv1m[1] -= o1;
        uv2p[1] += o2;
        uv2m[1] -= o2;
    }

    vec4 blurred = s0 * textureLod(BNB_SAMPLER_2D(tex_src), var_uv.xy, 0.)
                     + s1 * (textureLod(BNB_SAMPLER_2D(tex_src), uv1p, 0.) + textureLod(BNB_SAMPLER_2D(tex_src), uv1m, 0.))
                     + s2 * (textureLod(BNB_SAMPLER_2D(tex_src), uv2p, 0.) + textureLod(BNB_SAMPLER_2D(tex_src), uv2m, 0.));

    bnb_FragColor = blurred;
}
