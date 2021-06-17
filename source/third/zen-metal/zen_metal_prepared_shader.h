// used in metal/mm/cpp
#ifndef zen_metal_shader_h
#define zen_metal_shader_h

#include <simd/simd.h>

typedef struct {
	matrix_float4x4 matrix;
	packed_float4 color;
} VertexExtraIN;

typedef struct {
	packed_float2 pos;
	packed_float2 tex_pos;
} SamplerVertexIN;

typedef struct {
	packed_float4 color;
	packed_float2 pos;
} ColorVertexIN;

typedef struct {
	packed_float4 color;
	packed_float2 pos;
	float size;
} ParticleVertexIN;

#endif /* zen_metal_shader_h */
