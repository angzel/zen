#pragma once

#include "zen_vap2d_node.h"
#include "zen_vap2d_texture.h"
#include "zen_rect.h"
#include "zen_gles2_shader_color.h"
#include <vector>
#include <mutex>

namespace Zen { namespace Vap2d {

	using Zen::GL::EDrawMode;

	class Doodle : public LNode, public SizeView {

	public:
		Doodle(std::string const & name = "sprite");

		void addCoord(Zen::Point2, Zen::Color4f);

		size_t getCoordsCount();

		void clearCoords();

		void setDrawMode(Zen::GL::EDrawMode mode, float point_size);

		void setColor(Zen::Color4f color);
		Zen::Color4f getColor();

		virtual void draw() override;

	protected:
		struct Dot {
			Zen::Color4f color;
			Zen::Point2 coord;
		};
		Zen::GL::ArrayBuffer m_array_buffer;
		std::shared_ptr<Zen::GL::ShaderColor const> m_shader = nullptr;
		std::vector<Dot> m_buffer;
		Zen::Color4f m_color = Zen::EColor::White;
		Zen::GL::EDrawMode m_mode = Zen::GL::EDrawMode::Triangles;
		int m_mode_i = 2; // 0 points, 1 line, 2 triangle
		float m_point_size = 1;
		bool m_is_buffer_dirty = false;
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

		using Doodle::setColor;
		using Doodle::getColor;
	protected:
		using Doodle::addCoord;
		using Doodle::clearCoords;
		using Doodle::setDrawMode;
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

		void setLineWidth(float width);

		using Doodle::setColor;
		using Doodle::getColor;
	protected:
		using Doodle::addCoord;
		using Doodle::clearCoords;
		using Doodle::setDrawMode;
	};
}}
