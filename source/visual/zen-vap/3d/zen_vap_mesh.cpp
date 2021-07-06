#include "zen_vap_mesh.h"

namespace Zen { namespace Vap {
	std::shared_ptr<Mesh> MeshCube::GetDefaultCube()
	{
		static auto mesh = Create(0.5f);
		return mesh;
	}
	std::shared_ptr<Mesh> MeshCube::Create(float half_side)
	{
		return std::shared_ptr<Mesh>(new MeshCube(half_side));
	}
	MeshCube::MeshCube(float half_side)
	{
		this->mIndexes = {
			0, 1, 2, 2, 1, 3,
			4, 6, 5, 5, 6, 7,
			
			8,10,12,12,10,14,
			9,13,11,11,13,15,
			
			16, 20, 17, 17, 20, 21,
			18, 19, 22, 22, 19, 23,

		};
		this->mVertexes = {
			{ -half_side, -half_side, -half_side }, // 0
			{ +half_side, -half_side, -half_side }, // 1
			{ -half_side, +half_side, -half_side }, // 2
			{ +half_side, +half_side, -half_side }, // 3
			
			{ -half_side, -half_side, +half_side }, // 4
			{ +half_side, -half_side, +half_side }, // 5
			{ -half_side, +half_side, +half_side }, // 6
			{ +half_side, +half_side, +half_side }, // 7
			
				{ -half_side, -half_side, -half_side }, // 0
				{ +half_side, -half_side, -half_side }, // 1
				{ -half_side, +half_side, -half_side }, // 2
				{ +half_side, +half_side, -half_side }, // 3
				
				{ -half_side, -half_side, +half_side }, // 4
				{ +half_side, -half_side, +half_side }, // 5
				{ -half_side, +half_side, +half_side }, // 6
				{ +half_side, +half_side, +half_side }, // 7
			
			
				{ -half_side, -half_side, -half_side }, // 0
				{ +half_side, -half_side, -half_side }, // 1
				{ -half_side, +half_side, -half_side }, // 2
				{ +half_side, +half_side, -half_side }, // 3
				
				{ -half_side, -half_side, +half_side }, // 4
				{ +half_side, -half_side, +half_side }, // 5
				{ -half_side, +half_side, +half_side }, // 6
				{ +half_side, +half_side, +half_side }, // 7
			
			
//			{ -half_side, -half_side, -half_side }, // 0 -  8
//			{ -half_side, +half_side, -half_side }, // 2 - 10
//			{ -half_side, -half_side, +half_side }, // 4 - 12
//			{ -half_side, +half_side, +half_side }, // 6 - 14
//			
//			{ +half_side, -half_side, -half_side }, // 1 -  9
//			{ +half_side, +half_side, -half_side }, // 3 - 11
//			{ +half_side, -half_side, +half_side }, // 5 - 13
//			{ +half_side, +half_side, +half_side }, // 7 - 15
//			
//			{ -half_side, -half_side, -half_side }, // 0
//			{ +half_side, -half_side, -half_side }, // 1
//			{ -half_side, -half_side, +half_side }, // 4
//			{ +half_side, -half_side, +half_side }, // 5
//			
//			{ -half_side, +half_side, -half_side }, // 2
//			{ +half_side, +half_side, -half_side }, // 3
//			{ -half_side, +half_side, +half_side }, // 6
//			{ +half_side, +half_side, +half_side }, // 7
		};
//		float x1 = 0.25f;
//		float y1 = 1.f/3;
//		float x2 = 0.5f;
//		float x3 = 0.75f;
//		float y2 = 2.f/3;
		this->mTextureVertexes = {
			{},
		};
	}
}}
