// File for Metal kernel and shader functions

#include <metal_stdlib>
#include <simd/simd.h>

// Including header shared between this Metal shader code and Swift/C code executing Metal API commands
#import "zen_metal_prepared_shader.h"

using namespace metal;

#define VERTEX_BUF 0
#define EXTRA_BUF 1
#define F_TEXTURE_0 0

typedef struct {
	float4 position [[ position ]];
	float2 tex_pos;
	float4 color_add;
} SamplerInOut;

typedef struct {
	float4 position [[ position ]];
	float4 color;
	float4 color_add;
} ColorInOut;

typedef struct {
	float4 position [[ position ]];
	float4 color;
	float4 color_add;
	float point_size [[ point_size ]];
} ParticleInOut;

/**
 block: sampler
 */
vertex SamplerInOut VertexShaderSampler
(uint i [[ vertex_id ]],
 constant SamplerVertexIN * c [[ buffer(VERTEX_BUF) ]],
 constant VertexExtraIN & ex [[ buffer(EXTRA_BUF) ]]
 )
{
	SamplerInOut out;
	out.position = ex.matrix * float4(c[i].pos, 0, 1.0);
	out.tex_pos = c[i].tex_pos;
	out.color_add = ex.color;
	return out;
}
// rgba texture
fragment float4 FragmentShaderSampler_RGBA
(SamplerInOut in [[stage_in]],
texture2d<half> tex [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	return (float4)tex.sample(cs, in.tex_pos.xy) * in.color_add;
}
// convert rgb to grey
fragment float4 FragmentShaderSampler_RGBA_GA
(SamplerInOut in [[stage_in]],
texture2d<half> tex [[ texture(F_TEXTURE_0) ]])

{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);
	float4 c = (float4)tex.sample(cs, in.tex_pos.xy);
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, c.rgb);
	return float4(i, i, i, c.w) * in.color_add;
}
// alpha texture
fragment float4 FragmentShaderSampler_A
(SamplerInOut in [[stage_in]],
texture2d<half> tex [[ texture(F_TEXTURE_0) ]])

{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float i = (float)tex.sample(cs, in.tex_pos.xy).w;
	return float4(in.color_add.rgb, in.color_add.w * i);
}

fragment float4 FragmentShaderSampler_A_G
(SamplerInOut in [[stage_in]],
texture2d<half> tex [[ texture(F_TEXTURE_0) ]])

{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float i = (float)tex.sample(cs, in.tex_pos.xy).w;
	return float4(i, i, i, 1) * in.color_add;
}

/**
 color
 */

vertex ColorInOut VertexShaderColor
(uint i [[ vertex_id ]],
 constant ColorVertexIN * c [[ buffer(VERTEX_BUF) ]],
 constant VertexExtraIN & ex [[ buffer(EXTRA_BUF) ]]
 )
{
	ColorInOut out;
	out.position = ex.matrix * float4(c[i].pos, 0, 1.0);
	out.color = c[i].color;
	out.color_add = ex.color;
	return out;
}

fragment float4 FragmentShaderColor
(ColorInOut in [[stage_in]])
{
	return in.color * in.color_add;
}

fragment float4 FragmentShaderColorGrey
(ColorInOut in [[stage_in]])
{
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, in.color.rgb);
	return float4(i, i, i, in.color.w) * in.color_add;
}

/**
 particle
 */

vertex ParticleInOut VertexShaderParticle
(uint i [[ vertex_id ]],
 constant ParticleVertexIN * c [[ buffer(VERTEX_BUF) ]],
 constant VertexExtraIN & ex [[ buffer(EXTRA_BUF) ]]
 )
{
	ParticleInOut out;
	out.position = ex.matrix * float4(c[i].pos, 0, 1.0);
	out.color = c[i].color;
	out.point_size = c[i].size;
	out.color_add = ex.color;
	return out;
}

fragment float4 FragmentShaderParticle_RGBA
(ParticleInOut in [[stage_in]],
float2 co [[point_coord]],
texture2d<half> tex [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	return (float4)tex.sample(cs, co.xy) * in.color * in.color_add;
}

fragment float4 FragmentShaderParticle_RGBA_GA
(ParticleInOut in [[stage_in]],
float2 co [[point_coord]],
texture2d<half> tex [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);
	float4 c = (float4)tex.sample(cs, co.xy) * in.color;
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, c.rgb);
	return float4(i, i, i, c.w) * in.color_add;
}

fragment float4 FragmentShaderParticle_A
(ParticleInOut in [[stage_in]],
 float2 co [[point_coord]],
texture2d<half> tex [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float4 c = in.color * in.color_add;
	return float4(c.rgb, c.w * (float)tex.sample(cs, co.xy).w);
}

fragment float4 FragmentShaderParticle_A_G
(ParticleInOut in [[stage_in]],
 float2 co [[point_coord]],
texture2d<half> tex [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);
	
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, in.color.rgb) * (float)tex.sample(cs, co.xy).w;
	return float4(i, i, i, in.color.w) * in.color_add;
}
