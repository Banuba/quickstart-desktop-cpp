#include <bnb/glsl.frag>

BNB_IN(0) vec2 var_uv;

BNB_DECLARE_SAMPLER_2D(0, 1, camera);
BNB_DECLARE_SAMPLER_2D(2, 3, normalmap);

flat BNB_IN(1) vec2 rscale;

vec3 screen_blend( vec3 a, vec3 b )
{
    return 1. - ( 1. - a )*( 1. - b );
}
vec3 add_blend( vec3 a, vec3 b )
{
    return a + b;
}
// gamma to linear
vec3 g2l( vec3 g )
{
	return g*(g*(g*0.305306011+0.682171111)+0.012522878);
}

// combined hdr to ldr and linear to gamma
vec3 l2g( vec3 l )
{
	return sqrt(1.33*(1.-exp(-l)))-0.03;
}

void main()
{
    vec3 base = g2l(textureLod(BNB_SAMPLER_2D(camera), var_uv, 0.).xyz);
    vec4 n_mask = textureLod(BNB_SAMPLER_2D(normalmap), var_uv, 0.);

    vec3 N = normalize( n_mask.xyz*2. - 1. );

    vec2 r0 = (light0_pos.xy - var_uv)*rscale;
    float w0 = clamp( (light0_pos.w-length(r0))/(light0_pos.w-light0_pos.z), 0., 1. );

    vec2 r1 = (light1_pos.xy - var_uv)*rscale;
    float w1 = clamp( (light1_pos.w-length(r1))/(light1_pos.w-light1_pos.z), 0., 1. );

    vec3 light0 = base*g2l(light0_radiance.xyz)*(n_mask.w*w0*clamp(dot(N,normalize(vec3(r0,1.))),0.,1.));
    vec3 light1 = base*g2l(light1_radiance.xyz)*(n_mask.w*w1*clamp(dot(N,normalize(vec3(r1,1.))),0.,1.));

    vec3 color = base*ambient_multiplier.x;
    color = add_blend( color, light0 );
    color = add_blend( color, light1 );

    bnb_FragColor = vec4( l2g(color), 1. );
}
