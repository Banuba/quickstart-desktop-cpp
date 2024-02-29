#include <bnb/glsl.frag>
#include "soften.glsl"
#include "skin_color.glsl"

BNB_IN(0)
vec2 var_uv;
BNB_IN(1)
vec2 var_mask_uv;

BNB_DECLARE_SAMPLER_2D(0, 1, tex_camera);
BNB_DECLARE_SAMPLER_2D(2, 3, tex_mask);

float blendOverlay(float base, float blend) {
	return base<0.5?(2.0*base*blend):(1.0-2.0*(1.0-base)*(1.0-blend));
}

vec3 blendOverlay(vec3 base, vec3 blend) {
	return vec3(blendOverlay(base.r,blend.r),blendOverlay(base.g,blend.g),blendOverlay(base.b,blend.b));
}

vec3 blendOverlay(vec3 base, vec3 blend, float opacity) {
	return (blendOverlay(base, blend) * opacity + base * (1.0 - opacity));
}


vec3 adjustSkinTone(vec3 originalColor, vec3 desiredColor) {
  // Convert the original and desired colors to linear space.
  vec3 originalLinear = pow(originalColor, vec3(2.2));
  vec3 desiredLinear = pow(desiredColor, vec3(2.2));

  // Apply the overlay blend mode.
  vec3 blendedLinear = blendOverlay(originalLinear, desiredLinear);

  // Convert the blended color back to gamma space.
  return pow(blendedLinear, vec3(1.0 / 2.2));
}


void main()
{
    float mask = BNB_TEXTURE_2D(BNB_SAMPLER_2D(tex_mask), var_mask_uv).x * var_skin_color.a;
    vec4 softened = soften(BNB_PASS_SAMPLER_ARGUMENT(tex_camera), var_uv, var_skin_softening_strength.x);
    // vec4 colored = skin_color(softened, BNB_PASS_SAMPLER_ARGUMENT(tex_mask), var_mask_uv, var_skin_color);
    vec3 colored = adjustSkinTone(softened.rgb, var_skin_color.rgb);

    bnb_FragColor = vec4(colored.rgb, mask);
}
