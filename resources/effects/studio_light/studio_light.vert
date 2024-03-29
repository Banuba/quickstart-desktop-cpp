#include <bnb/glsl.vert>

BNB_LAYOUT_LOCATION(0)
BNB_IN vec3 attrib_pos;

BNB_DECLARE_SAMPLER_2D(0, 1, tex_warp);

BNB_OUT(0) vec2 var_uv;

flat BNB_OUT(1) vec2 rscale;

void main()
{
	vec2 v = attrib_pos.xy;
	gl_Position = vec4( v, 0., 1. );

	var_uv = v*0.5 + 0.5;
#if defined(BNB_VK_1)
	var_uv.y = 1. - var_uv.y;
#endif

	rscale = ( bnb_SCREEN.x > bnb_SCREEN.y )  ? vec2(1.,bnb_SCREEN.y/bnb_SCREEN.x) : vec2(bnb_SCREEN.x/bnb_SCREEN.y,1.);
}
