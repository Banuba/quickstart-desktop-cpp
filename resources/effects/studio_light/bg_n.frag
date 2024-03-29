#include <bnb/glsl.frag>

BNB_IN(0) vec4 var_uv;

BNB_DECLARE_SAMPLER_2D(0, 1, camera);
BNB_DECLARE_SAMPLER_2D(2, 3, bg_mask);
BNB_DECLARE_SAMPLER_2D(4, 5, bg_mask_blurred);

void main()
{
	float l = dot( textureLod( BNB_SAMPLER_2D(camera), var_uv.xy, 0. ).xyz, vec3( 0.299, 0.587, 0.114 ) );
	float m = 1. - textureLod( BNB_SAMPLER_2D(bg_mask), var_uv.zw, 0. ).x;
	float m_blur = 1. - textureLod( BNB_SAMPLER_2D(bg_mask_blurred), var_uv.zw, 0. ).x;

	l += clamp((m_blur-0.35)*m,0.0,1.0)*8.;

	float lx = dFdx(l);
	float ly = dFdy(l);

	vec3 n = normalize( vec3( -lx, -ly, 0.15 ) );

	const float bg_a = 0.2;
	const float body_a = 0.65;
	bnb_FragColor = vec4( n*0.5 + 0.5, mix( bg_a, body_a, m ) );
}
