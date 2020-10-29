//
#ifndef zen_metal_shader_h
#define zen_metal_shader_h

#define ZEN_SH_V 0
#define ZEN_SH_M 1
#define ZEN_SH_S0 0
#define ZEN_SH_S1 1
#define ZEN_SH_F_C 0
#include <simd/simd.h>

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
