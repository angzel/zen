#pragma once

#include "zen_vap_render.h"
#include "zen_vap_node.h"
#include "zen_vap_texture.h"
#include "zen_types.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap {

	class Doodle : public Node2d, public View2d, public Colorful {
	public:
		typedef VertexDataColors VertexData;

		Doodle(std::string const & name = "doodle");

	public:
		virtual void draw() override;
		
	protected:
		RBufferColors m_RBuf;
		std::vector<VertexData> m_dots;
		
		eVertexMode m_mode = eVertexMode::Line;
		Point2 m_view_scale;
		
		void initR_();
		void drawR_();
	};
	
	class FreeDoodle : public Doodle
	{
	public:
		FreeDoodle();

	public:
		void addPoint(Zen::Point2, Zen::Color4f);

		size_t getPointsCount();

		void removeAllPoints();

		void setPointPosition(int index, Zen::Point2);

		void setPointColor(int index, Zen::Color4f);

		Zen::Point2 getPointPosition(int index);

		Zen::Color4f getPointColor(int index);

		void setLineMode(bool strip);

		void setTriangleMode(bool strip);
	};

	class ColoredRect : public Doodle
	{
		/**
		 index valid value:[0~3].
		 by order:{ lb, rb, lt, rt }
		 */
	public:
		ColoredRect();

		void setRectSize(Size2 s);
		
		void setCoordColor(int index, Zen::Color4f color);

		Zen::Color4f getCoordColor(int index);
	};

	/**
	 a single line.
	 */
	class ColoredLine : public Doodle
	{
	public:
		ColoredLine();

		void setBeginColor(Zen::Color4f color);
		void setEndColor(Zen::Color4f color);
		void setBeginCoord(Zen::Point2 coord);
		void setEndCoord(Zen::Point2 coord);

		Zen::Color4f getBeginColor();
		Zen::Color4f getEndColor();
		Zen::Point2 getBeginCoord();
		Zen::Point2 getEndCoord();
	};
}}
