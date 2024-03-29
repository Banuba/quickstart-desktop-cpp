#include <bnb/glsl.vert>

layout(location = 0) in vec3 attrib_pos;
layout(location = 1) in vec3 attrib_pos_static;
layout(location = 2) in vec2 attrib_uv;
layout(location = 3) in vec4 attrib_red_mask;

BNB_OUT(1) vec4 var_uv;
BNB_OUT(1) vec3 var_v;

void main()
{
	gl_Position = bnb_MVP*vec4(attrib_pos,1.);
	mat3 basis = mat3(background_nn_transform);
	var_uv.xy = vec2(vec3(gl_Position.xy/gl_Position.w,1.) * basis);
	var_uv.zw = attrib_uv;
	var_v = (bnb_MV*vec4(attrib_pos,1.)).xyz;
}