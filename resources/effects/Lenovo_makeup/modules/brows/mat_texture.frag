#include <bnb/glsl.frag>


BNB_IN(0) vec2 var_uv;



BNB_DECLARE_SAMPLER_2D(0, 1, tex_diffuse);

void main()
{
	vec4 color = BNB_TEXTURE_2D(BNB_SAMPLER_2D(tex_diffuse),var_uv);

	color = color*var_brow_color;

	bnb_FragColor = vec4(color.rgb,color.a);
}
