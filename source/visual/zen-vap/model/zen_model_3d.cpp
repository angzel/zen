#include "zen_model_3d.h"

namespace Zen {
	Model3D::Model3D()
	{
		
	}
	
	bool Model3D::validate()
	{
		auto s = mPoints.size();
		for(auto i : mVertexIndexes)
		{
			if(i >= s) return false;
		}
		s = mVertexIndexes.size();
		for(auto i : mTriangleIndexes)
		{
			if(i >= s) return false;
		}
		return true;
	}
	
	struct ModelHeader {
		char sign[4];
		uint32_t pc; // point count
		uint32_t vi; // vertex index
		uint32_t ti; // triangle index
		char ex[64-16];
	};
	bool Model3D::ImportBin(std::vector<uint8_t> const & data)
	{
		ModelHeader mh;
		if(data.size() < sizeof(mh))
		{
			return false;
		}
		::memcpy((void*)&mh, data.data(), sizeof(mh)); ///< copy header.
		if(mh.pc > (1U<<24) || mh.vi > (1U<<24) || mh.ti > (1U<<24)) return false;
		
		// data size
		size_t pc_s = mh.pc * 3 * sizeof(float);
		size_t vi_s = mh.vi * sizeof(uint32_t);
		size_t ti_s = mh.ti * sizeof(uint32_t);
		size_t size = pc_s + vi_s + ti_s;
		if(data.size() < size + sizeof(mh))
		{
			return false;
		}
		size_t off = sizeof(mh);
		
		mPoints.resize(mh.pc);
		::memcpy(mPoints.data(), data.data() + off, pc_s);
		off += pc_s;

		mVertexIndexes.resize(mh.vi);
		::memcpy(mVertexIndexes.data(), data.data() + off, vi_s);
		off += vi_s;

		mTriangleIndexes.resize(mh.ti);
		::memcpy(mTriangleIndexes.data(), data.data() + off, ti_s);
		
		return validate();
	}
	
	bool Model3D::ImportText(std::string const & source)
	{
		std::string s;
		for(auto c : source)
		{
			if(!isspace(c)) s.push_back(c);
		}
		std::vector<std::string> vp1;
		return false;
	}
	
	std::vector<uint8_t> Model3D::ExportBin()
	{
		if(!validate()) return {};
		auto pcs = mPoints.size() * sizeof(float) * 3;
		auto vis = mVertexIndexes.size() * sizeof(uint32_t);
		auto tis = mTriangleIndexes.size() * sizeof(uint32_t);
		auto size = pcs + vis + tis + sizeof(ModelHeader);
		std::vector<uint8_t> data(size);
		auto dest = data.data();

		ModelHeader mh;
		mh.pc = (uint32_t)mPoints.size();
		mh.vi = (uint32_t)mVertexIndexes.size();
		mh.ti = (uint32_t)mTriangleIndexes.size();
		::memcpy(dest, (void*)&mh, sizeof(mh));
		dest += sizeof(mh);
		::memcpy(dest, (void*)mPoints.data(), pcs);
		dest += pcs;
		::memcpy(dest, (void*)mVertexIndexes.data(), vis);
		dest += vis;
		::memcpy(dest, (void*)mTriangleIndexes.data(), tis);
		
		return data;
	}
	
	std::string Model3D::ExportText()
	{
		if(!validate()) return "";
		return "";
	}
}
