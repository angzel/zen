#pragma once

#include <vector>
#include <string>
#include "zen_vector.h"

namespace Zen {
	/**
	 @class Model3D
	 @brief import or export the mesh data of a 3d model.
	 @note a text format is defined as:
	 [points set A][vertex indexes set B][triangle indexes set C]
	 A: "[(f,f,f)(f,f,f)...]", each "(f,f,f)" means a 3d point, "f" is a float number like "12.234"
	 B: "[n,n...]", "n" is a integer means the index of a point in A. so we will get a B-point-set
	 C: "[(a,b,c)(a,b,c)...]", "a,b,c" are the point indexes in B-point-set, and they make a triangle.
	 @example an example string is like this:
	 "[(0,0,0)(1,0,0)(0,1.0,0.0)][0,1,2,0,1,2][(0,1,2)(4,3,5)]"
	 this is a triangle with two faces, we can bind diffrent texture coords for the faces.
	 */
	class Model3D
	{
	public:
		Model3D();
		
		bool validate();
		
		bool ImportBin(std::vector<uint8_t> const & data);
		
		bool ImportText(std::string const & source);
		
		std::vector<uint8_t> ExportBin();
		
		std::string ExportText();
		
		std::vector<Vector3> & points() {
			return mPoints;
		}
		
		std::vector<uint32_t> & vertexIndexes() {
			return mVertexIndexes;
		}
		
		std::vector<uint32_t> & triangleIndexes() { return mTriangleIndexes; }
		
	protected:
		std::vector<Vector3> mPoints;
		std::vector<uint32_t> mVertexIndexes;
		std::vector<uint32_t> mTriangleIndexes;
	};
}
