#include <bnb/glsl.vert>

BNB_LAYOUT_LOCATION(0) BNB_IN vec2 attrib_pos;

BNB_OUT(0) vec4 var_uv;

void main()
{
	vec2 v = attrib_pos.xy;
	gl_Position = vec4( v, 0., 1. );
	var_uv.xy = v*0.5 + 0.5;
	mat3 basis = mat3(background_nn_transform);
	var_uv.zw = vec2(vec3(v,1.) * basis);
}
