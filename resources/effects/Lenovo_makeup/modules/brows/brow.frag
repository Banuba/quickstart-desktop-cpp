#include <bnb/glsl.frag>

#define BNB_USE_UVMORPH


BNB_IN(0) vec2 var_uv;
BNB_IN(1) vec2 var_bg_uv;


BNB_DECLARE_SAMPLER_2D(2, 3, camera);

BNB_DECLARE_SAMPLER_2D(0, 1, tex_a);


void main()
{
	bnb_FragColor = vec4( BNB_TEXTURE_2D(BNB_SAMPLER_2D(camera), var_bg_uv ).xyz, BNB_TEXTURE_2D(BNB_SAMPLER_2D(tex_a), var_uv ).x );
}
