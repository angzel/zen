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
#import "ShaderTypes.h"

using namespace metal;

//typedef struct
//{
//    float3 position [[attribute(VertexAttributePosition)]];
//    float2 texCoord [[attribute(VertexAttributeTexcoord)]];
//} Vertex;

typedef struct
{
    float4 position [[position]];
//	float point_size [[ point_size ]];
    float2 texCoord;
} ColorInOut;

vertex ColorInOut vertexShader1(uint i [[ vertex_id ]],
//								in [[stage_in]]
								constant float2 * position [[ buffer(0) ]],
								constant float2 * texCoord [[ buffer(1) ]]
								)
//,
//                               constant Uniforms & uniforms [[ buffer(BufferIndexUniforms) ]])
{
    ColorInOut out;

//    float4 position = float4(in.position, 1.0);
    out.position = float4(position[i], 0, 1.0);
//    out.texCoord = texCoord[i];
	out.texCoord = texCoord[i];

    return out;
}

fragment float4 fragmentShader1(ColorInOut in [[stage_in]],
//                               constant Uniforms & uniforms [[ buffer(BufferIndexUniforms) ]],
                               texture2d<half> colorMap [[ texture(0) ]])
//								float2 pointCoord [[point_coord]])
{
    constexpr sampler colorSampler(mip_filter::linear,
                                   mag_filter::linear,
                                   min_filter::linear);

    half4 colorSample   = colorMap.sample(colorSampler, in.texCoord.xy);
	float4 c = float4(colorSample);
	return c;
}
