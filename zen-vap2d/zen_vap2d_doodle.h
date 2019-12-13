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
		Doodle(std::string const & name = "doodle");

		void addCoord(Zen::Point2, Zen::Color4f);

		size_t getCoordsCount();

		void clearCoords();

		void setLineMode(bool strip);

		void setTriangleMode(bool strip);

		virtual void draw() override;

		void setAreaSize(Size2 size);

		Size2 getAreaSize();
	protected:
		struct Dot {
			Zen::Color4f color;
			Zen::Point2 coord;
		};
		DoodleBuffer m_gpus;
		std::vector<Dot> m_dots;
		eMode m_mode = eMode::Point;
		bool m_is_buffer_dirty = false;

		void _initDoodle();
		void _drawDoodle();
	};

	class DoodleColors : public Doodle
	{
	public:
		DoodleColors();
		/**
		 index valid value:[0~3]
		 */
		void setCoordColor(int index, Zen::Color4f color);
		Zen::Color4f getCoordColor(int index);

	protected:
		using Doodle::addCoord;
		using Doodle::clearCoords;
		using Doodle::setLineMode;
		using Doodle::setTriangleMode;
	};

	/**
	 a single line.
	 */
	class DoodleLine : public Doodle
	{
	public:
		DoodleLine();

		void setBeginColor(Zen::Color4f color);
		void setEndColor(Zen::Color4f color);
		void setBeginCoord(Zen::Point2 coord);
		void setEndCoord(Zen::Point2 coord);
		Zen::Color4f getBeginColor();
		Zen::Color4f getEndColor();
		Zen::Point2 getBeginCoord();
		Zen::Point2 getEndCoord();

	protected:
		using Doodle::addCoord;
		using Doodle::clearCoords;
		using Doodle::setLineMode;
		using Doodle::setTriangleMode;
	};
}}
