#include <bnb/glsl.frag>

BNB_DECLARE_SAMPLER_2D(0, 1, hair_mask);
BNB_DECLARE_SAMPLER_2D(2, 3, edge_mask);

BNB_IN(0) vec4 var_uv;
BNB_IN(1) vec3 var_v;

void main()
{
	float h = textureLod( BNB_SAMPLER_2D(hair_mask), var_uv.xy, 0. ).x;
	float e = textureLod( BNB_SAMPLER_2D(edge_mask), var_uv.zw, 0. ).x;
	vec3 n = normalize( cross( dFdx(var_v), dFdy(var_v) ) );
	bnb_FragColor = vec4( n*0.5 + 0.5, (1. - h)*e );
}