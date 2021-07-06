// used in metal/mm/cpp
#ifndef zen_metal_shader_h
#define zen_metal_shader_h

#include <simd/simd.h>

typedef struct {
	matrix_float4x4 matrix;
	packed_float4 color;
} UniformIN_Shader;

#define UniformIN_Shader_M_OFF 0
#define UniformIN_Shader_C_OFF (sizeof(float) * 16)
#define UniformIN_Shader_SIZE (sizeof(float) * 20)

typedef struct {
	matrix_float4x4 matrix;
	packed_float4 color;
	packed_float4 size_ratio;
} UniformIN_ShaderP;

#define UniformIN_ShaderP_M_OFF 0
#define UniformIN_ShaderP_C_OFF (sizeof(float) * 16)
#define UniformIN_ShaderP_S_OFF (sizeof(float) * 20)
#define UniformIN_ShaderP_SIZE (sizeof(float) * 24)

typedef struct {
	packed_float2 coord;
	packed_float2 tt_coord;
} VertexIN_ShaderT;

typedef struct {
	packed_float2 coord;
	packed_float4 color;
} VertexIN_ShaderC;

typedef struct {
	packed_float2 coord;
	packed_float4 color;
	float psize;
} VertexIN_ShaderP;

#endif /* zen_metal_shader_h */
