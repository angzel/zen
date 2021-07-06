#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include "zen_vap_texture.h"
#include "zen_types.h"
#include "zen_exception.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {
	class Figure : public Node2d, public View2d, public Colorful
	{
	protected:
		Figure(std::string const & name)
		: Node2d(name)
		{
			
		}
	};
	
	template<class tDtataType>
	class FigureBase_ : public Figure
	{
	public:
		typedef tDtataType VertexData;
	protected:
		std::vector<VertexData> m_dots;
		Point2 m_view_scale;
		bool m_is_vertex_dirty = true;
	protected:
		FigureBase_(std::string const & name)
		: Figure(name)
		{
			
		}
	public:
		size_t getVertexesCount()
		{
			return m_dots.size();
		}
		
		void setVertexes(VertexData const * src, size_t count) {
			m_dots.assign(src, src+count);
			m_is_vertex_dirty = true;
		}
		
		void updateVertexes(size_t index, VertexData const * src, size_t count) {

			for(size_t i = 0; i < count; ++i)
			{
				m_dots[i+index] = *src++;
			}
			m_is_vertex_dirty = true;
		}
		
		void updateVertex(size_t index, VertexData const & data) {
			m_dots[index] = data;
			m_is_vertex_dirty = true;
		}
		
		void addVertex(VertexData const & data) {
			m_dots.push_back(data);
			m_is_vertex_dirty = true;
		}
		
		VertexData const & getVertex(size_t index) const {
			return m_dots[index];
		}
	};

	class FigurePoints : public FigureBase_<VertexDataPoints>
	{
	public:
		FigurePoints(std::string const & name = "figure points");

		virtual void draw() override;
		
		void setTexture(TextureShared texture);
		
		TextureShared getTexture();
		
	protected:
		TextureShared m_texture;
		
		bool m_is_texture_dirty = true;
	protected:
		RBufferPoints m_RBuf;
		Vector4 m_size_ratio;
		void initR_();
		void drawR_();
		void updateSize_();
	};
	
	class FigureColors : public FigureBase_<VertexDataColors>
	{
	public:
		FigureColors(std::string const & name = "FigureColors");
		
		virtual void draw() override;

		void setStrip(bool);

		bool isStrip();
	protected:
		RBufferColors m_RBuf;
		bool m_is_strip = false;
		void initR_();
		void drawR_();
	};
	
	class FigureLines : public FigureBase_<VertexDataColors>
	{
	public:
		FigureLines(std::string const & name = "figure lines");
		
		virtual void draw() override;
		
		void setStrip(bool);
		
		bool isStrip();
	protected:
		RBufferColors m_RBuf;
		bool m_is_strip = false;
		void initR_();
		void drawR_();
	};

	class FigureTexture : public FigureBase_<VertexDataTexture>
	{
	public:
		FigureTexture(std::string const & name = "FigureTexture");
		
		virtual void draw() override;
		
		void setStrip(bool);

		bool isStrip();
		
		void setTexture(TextureShared texture);
		TextureShared getTexture();

		void setTextureRect(Zen::Rect4f rect);
		Zen::Rect4f getTextureRect();

	protected:
		TextureShared m_texture;
		
		Zen::Rect4f m_texture_rect = { 0, 0, 1, 1 };
		
		bool m_is_texture_dirty = true;
	protected:
		RBufferPoints m_RBuf;
		void initR_();
		void drawR_();
		void updateSize_();
	};
}}
