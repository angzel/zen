#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include "zen_vap_texture.h"
#include "zen_types.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {
	class Mesh
	{
	public:
		std::vector<Vector3> const & getVertexes() const {
			return mVertexes;
		}
		std::vector<uint32_t> const & getIndexes() const {
			return mIndexes;
		}
	protected:
		std::vector<Vector3> mVertexes;
		std::vector<Vector3> mTextureVertexes;
		std::vector<uint32_t> mIndexes;
	};
	
	class MeshCube : public Mesh {
	public:
		MeshCube(float half_side);
		// length of side is 1, center is (0,0,0).
		static std::shared_ptr<Mesh> GetDefaultCube();
		static std::shared_ptr<Mesh> Create(float half_side);
	};
}}
