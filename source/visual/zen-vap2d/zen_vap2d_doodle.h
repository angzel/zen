#pragma once

#include "zen_vap2d_config.h"
#include "zen_vap2d_node.h"
#include "zen_vap2d_texture.h"
#include "zen_rect.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {

	class Doodle : public FinalNode, public SizeView, public Colorful {
	public:
	public:
		Doodle(std::string const & name = "doodle");

	public:
		virtual void draw() override;

		void setAreaSize(Size2 size);

		Size2 getAreaSize();

		void setDotsDirty();
		
	protected:
		struct Dot {
			Zen::Color4f color;
			Zen::Point2 coord;
		};
		DoodleBuffer m_gpus;
		std::vector<Dot> m_dots;
		
		eVertexMode m_mode = eVertexMode::Line;

		bool m_is_buffer_dirty = false;

		void _initDoodle();
		void _drawDoodle();
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
