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
	float2 tt_coord;
	float4 color_add;
} ShaderTInOut;

typedef struct {
	float4 position [[ position ]];
	float4 color;
	float4 color_add;
} ShaderCInOut;

typedef struct {
	float4 position [[ position ]];
	float4 color;
	float4 color_add;
	float point_size [[ point_size ]];
} ShaderPInOut;


vertex ShaderTInOut VShaderT
(uint i [[ vertex_id ]],
 constant VertexIN_ShaderT * v [[ buffer(VERTEX_BUF) ]],
 constant UniformIN_Shader & u [[ buffer(EXTRA_BUF) ]]
 )
{
	ShaderTInOut out;
	out.position = u.matrix * float4(v[i].coord, 0, 1.0);
	out.tt_coord = v[i].tt_coord;
	out.color_add = u.color;
	return out;
}
// rgba texture
fragment float4 FShaderT_RGBA
(ShaderTInOut in [[stage_in]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	return (float4)t.sample(cs, in.tt_coord.xy) * in.color_add;
}
// convert rgb to grey
fragment float4 FShaderT_RGBA_GA
(ShaderTInOut in [[stage_in]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])

{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);
	float4 c = (float4)t.sample(cs, in.tt_coord.xy);
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, c.rgb);
	return float4(i, i, i, c.w) * in.color_add;
}
// alpha texture
fragment float4 FShaderT_A
(ShaderTInOut in [[stage_in]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float i = (float)t.sample(cs, in.tt_coord.xy).w;
	return float4(in.color_add.rgb, in.color_add.w * i);
}

fragment float4 FShaderT_A_G
(ShaderTInOut in [[stage_in]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])

{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float i = (float)t.sample(cs, in.tt_coord.xy).w;
	return float4(i, i, i, 1) * in.color_add;
}

/**
 color
 */

vertex ShaderCInOut VShaderC
(uint i [[ vertex_id ]],
 constant VertexIN_ShaderC * v [[ buffer(VERTEX_BUF) ]],
 constant UniformIN_Shader & u [[ buffer(EXTRA_BUF) ]]
 )
{
	ShaderCInOut out;
	out.position = u.matrix * float4(v[i].coord, 0, 1.0);
	out.color = v[i].color;
	out.color_add = u.color;
	return out;
}

fragment float4 FShaderC
(ShaderCInOut in [[stage_in]])
{
	return in.color * in.color_add;
}

fragment float4 FShaderC_G
(ShaderCInOut in [[stage_in]])
{
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, in.color.rgb);
	return float4(i, i, i, in.color.w) * in.color_add;
}

/**
 particle
 */

vertex ShaderPInOut VShaderP
(uint i [[ vertex_id ]],
 constant VertexIN_ShaderP * v [[ buffer(VERTEX_BUF) ]],
 constant UniformIN_ShaderP & u [[ buffer(EXTRA_BUF) ]]
 )
{
	ShaderPInOut out;
	out.position = u.matrix * float4(v[i].coord, 0, 1.0);
	out.color = v[i].color;
	out.point_size = length(u.matrix * float4(v[i].psize, 0.0, 0.0, 0.0) * u.size_ratio);
	out.color_add = u.color;
	return out;
}

fragment float4 FShaderP
(ShaderPInOut in [[stage_in]],
float2 p [[point_coord]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])
{
	return in.color * in.color_add;
}

fragment float4 FShaderP_G
(ShaderPInOut in [[stage_in]],
 float2 p [[point_coord]])
{
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, in.color.rgb);
	return float4(i, i, i, in.color.w) * in.color_add;
}

fragment float4 FShaderPT_RGBA
(ShaderPInOut in [[stage_in]],
float2 p [[point_coord]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	return (float4)t.sample(cs, p.xy) * in.color * in.color_add;
}

fragment float4 FShaderPT_RGBA_GA
(ShaderPInOut in [[stage_in]],
float2 p [[point_coord]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);
	float4 c = (float4)t.sample(cs, p.xy) * in.color;
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, c.rgb);
	return float4(i, i, i, c.w) * in.color_add;
}

fragment float4 FShaderPT_A
(ShaderPInOut in [[stage_in]],
 float2 p [[point_coord]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float4 c = in.color * in.color_add;
	return float4(c.rgb, c.w * (float)t.sample(cs, p.xy).w);
}

fragment float4 FShaderPT_A_G
(ShaderPInOut in [[stage_in]],
 float2 p [[point_coord]],
texture2d<half> t [[ texture(F_TEXTURE_0) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);
	
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, in.color.rgb) * (float)t.sample(cs, p.xy).w;
	return float4(i, i, i, in.color.w) * in.color_add;
}
