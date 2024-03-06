#include <bnb/glsl.vert>
#include <bnb/decode_int1010102.glsl>
#include<bnb/matrix_operations.glsl>
#define bnb_IDX_OFFSET 0
#ifdef BNB_VK_1
#ifdef gl_VertexID
#undef gl_VertexID
#endif
#ifdef gl_InstanceID
#undef gl_InstanceID
#endif
#define gl_VertexID gl_VertexIndex
#define gl_InstanceID gl_InstanceIndex
#endif

const float UV_MORPH_STR = 0.5;
#define BNB_USE_AUTOMORPH
#define BNB_USE_UVMORPH


BNB_LAYOUT_LOCATION(0) BNB_IN vec3 attrib_pos;
BNB_LAYOUT_LOCATION(3) BNB_IN vec2 attrib_uv;
#ifndef BNB_GL_ES_1
BNB_LAYOUT_LOCATION(4) BNB_IN uvec4 attrib_bones;
#else
BNB_LAYOUT_LOCATION(4) BNB_IN vec4 attrib_bones;
#endif
BNB_LAYOUT_LOCATION(5) BNB_IN vec4 attrib_weights;



BNB_DECLARE_SAMPLER_2D(4, 5, bnb_BONES);

BNB_DECLARE_SAMPLER_2D(6, 7, bnb_UVMORPH);

BNB_DECLARE_SAMPLER_2D(8, 9, bnb_STATICPOS);

BNB_DECLARE_SAMPLER_2D(10, 11, bnb_MORPH);


BNB_OUT(1) vec2 var_bg_uv;
BNB_OUT(0) vec2 var_uv;

#include <bnb/anim_transform.glsl>
#include <bnb/morph_transform.glsl>
#include <bnb/get_bone.glsl>

BNB_DECLARE_SAMPLER_2D(2, 3, tex_bg_mask);

invariant gl_Position;

float sample_bg_mask(vec3 vpos)
{
    vec4 v = (bnb_PROJ*bnb_MV_[2])*vec4(vpos,1.);
    vec2 mask_uv = (vec3(v.xy/v.w,1.)*mat3(background_nn_transform)).xy;
    float mask = textureLod(BNB_SAMPLER_2D(tex_bg_mask),mask_uv,0).x;
    return mask;
}

void main()
{
    mat4 m = bnb_get_transform();
    vec3 vpos = attrib_pos;

#ifdef BNB_VK_1
    vec2 flip_uv = vec2( attrib_uv.x, 1. - attrib_uv.y );
#else
    vec2 flip_uv = vec2( attrib_uv.xy );
#endif

#ifdef BNB_USE_UVMORPH
    const float max_range = 40.;
    vec3 translation = BNB_TEXTURE_2D(BNB_SAMPLER_2D(bnb_UVMORPH), smoothstep(0.,1.,flip_uv)).xyz*(2.*max_range) - max_range;
#ifdef GLFX_UVMORPH_Z_UP
    vpos += vec3(translation.x,-translation.z,translation.y);
#else
    vpos += translation  * UV_MORPH_STR;
    // vpos.y += 30.;
#endif
#endif

    vpos = vec3(vec4(vpos,1.)*m);

#ifdef BNB_USE_AUTOMORPH
#ifndef BNB_AUTOMORPH_BONE
    vpos = bnb_auto_morph( vpos );
#else
    vpos = bnb_auto_morph_bone( vpos, m );
#endif
#endif

// #ifndef BNB_VK_1
// #else
    // vec2 flip_uv = vec2( attrib_uv.xy );
// #endif

    // vec3 translation = BNB_TEXTURE_2D(BNB_SAMPLER_2D(bnb_UVMORPH),flip_uv).xyz;
    // vpos += translation

    gl_Position = bnb_MVP * vec4(vpos,1.);

	flip_uv.y = 1. - flip_uv.y;

    var_uv = attrib_uv;
    vec4 uvmorphed_view = bnb_MVP * vec4( BNB_TEXTURE_2D(BNB_SAMPLER_2D(bnb_STATICPOS),flip_uv).xyz + translation, 1. );
    #ifndef BNB_VK_1
var_bg_uv = (uvmorphed_view.xy/uvmorphed_view.w)*0.5 + 0.5;
#else
var_bg_uv = (uvmorphed_view.xy/uvmorphed_view.w)*0.5 + 0.5;
var_bg_uv.y = 1. - var_bg_uv.y;
#endif

	var_uv = attrib_uv;
}
