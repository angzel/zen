//
//  Shaders.metal
//  MetalTest
//
//  Created by lover on 2019/12/13.
//  Copyright © 2019 lover. All rights reserved.
//

// File for Metal kernel and shader functions

#include <metal_stdlib>
#include <simd/simd.h>

// Including header shared between this Metal shader code and Swift/C code executing Metal API commands
#import "zen_metal_shader.h"

using namespace metal;

/**
 sampler
 */
typedef struct {
	float4 position [[ position ]];
	float2 tex_pos;
} SamplerInOut;

vertex SamplerInOut VertexShaderSampler
(uint i [[ vertex_id ]],
 constant SamplerVertexIN * c [[ buffer(ZEN_SH_V) ]],
 constant matrix_float4x4 & m [[ buffer(ZEN_SH_M) ]]
 )
{
	SamplerInOut out;
	out.position = m * float4(c[i].pos, 0, 1.0);
	out.tex_pos = c[i].tex_pos;
	return out;
}

fragment float4 FragmentShaderSampler
(SamplerInOut in [[stage_in]],
texture2d<half> tex [[ texture(ZEN_SH_S0) ]],
constant float4 & color [[ buffer(ZEN_SH_F_C) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	return (float4)tex.sample(cs, in.tex_pos.xy) * color;
}

fragment float4 FragmentShaderSamplerGrey
(SamplerInOut in [[stage_in]],
texture2d<half> tex [[ texture(ZEN_SH_S0) ]],
constant float4 & color [[ buffer(ZEN_SH_F_C) ]])

{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);
	float4 c = (float4)tex.sample(cs, in.tex_pos.xy);
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, c.rgb);
	return float4(i, i, i, c.w) * color;
}

fragment float4 FragmentShaderSamplerAlpha
(SamplerInOut in [[stage_in]],
texture2d<half> tex [[ texture(ZEN_SH_S0) ]],
constant float4 & color [[ buffer(ZEN_SH_F_C) ]])

{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float i = (float)tex.sample(cs, in.tex_pos.xy).w;
	return float4(1, 1, 1, i) * color;
}

/**
 color
 */

typedef struct {
	float4 position [[ position ]];
	float4 color;
//	float size [[ point_size ]];
} ColorInOut;

vertex ColorInOut VertexShaderColor
(uint i [[ vertex_id ]],
 constant ColorVertexIN * c [[ buffer(ZEN_SH_V) ]],
 constant matrix_float4x4 & m [[ buffer(ZEN_SH_M) ]]
 )
{
	ColorInOut out;
	out.position = m * float4(c[i].pos, 0, 1.0);
	out.color = c[i].color;
//	out.size = 100;
	return out;
}

fragment float4 FragmentShaderColor
(ColorInOut in [[stage_in]],
 constant float4 & color [[ buffer(ZEN_SH_F_C) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	return in.color * color;
}

fragment float4 FragmentShaderColorGrey
(ColorInOut in [[stage_in]],
 constant float4 & color [[ buffer(ZEN_SH_F_C) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, in.color.rgb);
	return float4(i, i, i, in.color.w) * color;
}


/**
 particle
 */

typedef struct {
	float4 position [[ position ]];
	float4 color;
	float point_size [[ point_size ]];
} ParticleInOut;

vertex ParticleInOut VertexShaderParticle
(uint i [[ vertex_id ]],
 constant ParticleVertexIN * c [[ buffer(ZEN_SH_V) ]],
 constant matrix_float4x4 & m [[ buffer(ZEN_SH_M) ]]
 )
{
	ParticleInOut out;
	out.position = m * float4(c[i].pos, 0, 1.0);
	out.color = c[i].color;
	out.point_size = c[i].size;
	return out;
}

fragment float4 FragmentShaderParticle
(ParticleInOut in [[stage_in]],
float2 co [[point_coord]],
texture2d<half> tex [[ texture(ZEN_SH_S0) ]],
constant float4 & color [[ buffer(ZEN_SH_F_C) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	return (float4)tex.sample(cs, co.xy) * in.color * color;
}

fragment float4 FragmentShaderParticleGrey
(ParticleInOut in [[stage_in]],
float2 co [[point_coord]],
texture2d<half> tex [[ texture(ZEN_SH_S0) ]],
constant float4 & color [[ buffer(ZEN_SH_F_C) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);
	float4 c = (float4)tex.sample(cs, co.xy);
	float3 grey = float3(0.21, 0.71, 0.08);
	float i = dot(grey, c.rgb);
	return float4(i, i, i, c.w) * in.color * color;
}

fragment float4 FragmentShaderParticleAlpha
(ParticleInOut in [[stage_in]],
 float2 co [[point_coord]],
texture2d<half> tex [[ texture(ZEN_SH_S0) ]],
constant float4 & color [[ buffer(ZEN_SH_F_C) ]])
{
	constexpr sampler
	cs(mip_filter::linear, mag_filter::linear, min_filter::linear);

	float i = (float)tex.sample(cs, co.xy).w;
	return float4(1, 1, 1, i) * in.color * color;
}
