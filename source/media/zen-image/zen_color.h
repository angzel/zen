/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include <cstdint>

namespace Zen 
{
	enum class ePixel : int
	{
		None  = 0,
		Grey  = 1,	// grey 1 byte
		GA    = 2,	// grey+alpha 2 bytes
		RGB   = 3,	// 3 bytes
		RGBA  = 4,	// 4 bytes
	};
	
	class Color4b;
	class Color4f;
	class ColorRBG;
	
	/**
	 enum usual color's name in RGB format.
	 */
	enum class eColor : uint32_t
	{
		Black               = 0x000000,
		White               = 0xFFFFFF,
		Grey                = 0x808080,
		Red                 = 0xFF0000,
		Green               = 0x00FF00,
		Blue                = 0x0000FF,
		Cyan                = 0x00FFFF,
		Magenta             = 0xFF00FF,
		Yellow              = 0xFFFF00,
	};

	class Color3b
	{
	public:
		uint8_t r_byte, g_byte, b_byte;
	public:
		Color3b()
		{
			r_byte = g_byte = b_byte = 0;
		}
		Color3b(eColor rgb)
		{
			this->set((uint32_t)rgb);
		}
		Color3b(uint32_t rgb)
		{
			this->set(rgb);
		}
		Color3b(uint8_t r, uint8_t g, uint8_t b)
		{
			this->set(r, g, b);
		}

		void set(uint32_t rgb)
		{
			b_byte = rgb & 0xff;
			g_byte = (rgb >> 8) & 0xff;
			r_byte = (rgb >> 16) & 0xff;
		}
		void set(uint8_t r, uint8_t g, uint8_t b)
		{
			r_byte = r;
			g_byte = g;
			b_byte = b;
		}
		uint32_t getRGB() const
		{
			return (r_byte << 16) | (g_byte << 8) | b_byte;
		}
		
		bool operator == (Color3b const & o) const
		{
			return r_byte == o.r_byte && g_byte == o.g_byte && b_byte == o.b_byte;
		}
		
		bool operator != (Color3b const & o) const
		{
			return !(*this == o);
		}
	};


	/* C: Color4b
	 - 32bits Color4b in RGBA format, 0xRRGGBBAA.
	 */


	class Color4b
	{
	public:
		uint8_t r_byte, g_byte, b_byte, a_byte;
	public:
		Color4b(eColor e)
		{
			set(Color3b(e));
		}
		/* 0xRRGGBBAA */
		Color4b(uint32_t rgba = 0x0FF)
		{
			setRGBA(rgba);
		}
		
		Color4b(Color3b const & rgb, uint8_t a = 0xFF)
		{
			set(rgb, a);
		}

		Color4b(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
		{
			set(r, g, b, a);
		}

		inline Color4b(Color4f const & cf);

		inline void set(Color4f const &);

		void set(Color3b const & rgb, uint8_t a = 0xFF)
		{
			r_byte = rgb.r_byte;
			g_byte = rgb.g_byte;
			b_byte = rgb.b_byte;
			a_byte = a;
		}

		void set(eColor value, uint8_t a = 0xFF)
		{
			this->set(Color3b(value), a);
		}
		
		void set(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
		{
			r_byte = r;
			g_byte = g;
			b_byte = b;
			a_byte = a;
		}

		void set(uint32_t rgba)
		{
			this->setRGBA(rgba);
		}

		/* 0xRRGGBBAA */
		void setRGBA(uint32_t rgba)
		{
			r_byte = rgba >> 24;
			g_byte = (rgba & 0xff0000) >> 16;
			b_byte = (rgba & 0xff00) >> 8;
			a_byte = rgba & 0xff;
		}
		
		/* 0xAARRGGBB */
		void setARGB(uint32_t argb)
		{
			a_byte = argb >> 24;
			r_byte = (argb & 0xff0000) >> 16;
			g_byte = (argb & 0xff00) >> 8;
			b_byte = argb & 0xff;
		}
		
		void setAlpha(uint8_t a)
		{
			a_byte = a;
		}

		void setGrey(uint8_t grey)
		{
			r_byte = g_byte = b_byte = grey;
		}
		
		uint32_t getARGB() const
		{
			return ((uint32_t)a_byte<< 24) | ((uint32_t)r_byte << 16) |
			((uint32_t)g_byte << 8) | ((uint32_t)b_byte);
		}
		
		uint32_t getRGBA() const
		{
			return ((uint32_t)r_byte << 24) | ((uint32_t)g_byte << 16) |
			((uint32_t)b_byte << 8) | ((uint32_t)a_byte);
		}
		
		bool operator == (Color4b const & o)
		{
			return this->getARGB() == o.getARGB();
		}
		
		bool operator != (Color4b const & o)
		{
			return !(*this == o);
		}

		float af() const
		{
			return a_byte / (float)0xff;
		}
		float rf() const
		{
			return r_byte / (float)0xff;
		}
		float gf() const
		{
			return g_byte / (float)0xff;
		}
		float bf() const
		{
			return b_byte / (float)0xff;
		}

		operator Color3b ()
		{
			return Color3b(r_byte, g_byte, b_byte);
		}
	};
	

	 
	/** Color4f
	 -- Color4b by 4 float
	 */
	class Color4f
	{
	public:
		float red, green, blue, alpha;
		
	public:
		Color4f(eColor e)
		{
			set(Color4b(e));
		}
		
		Color4f()
		{
			red = green = blue = 0.f;
			alpha = 1.f;
		}

		Color4f(Color4b const & color)
		{
			set(color);
		}

		Color4f(float r, float g, float b, float a)
		{
			set(r, g, b, a);
		}
		
		void set(float r, float g, float b, float a)
		{
			red = r;
			green = g;
			blue = b;
			alpha = a;
		}
		
		void set(Color4b const & color)
		{
			red = color.rf();
			green = color.gf();
			blue = color.bf();
			alpha = color.af();
		}
		void operator *= (Color4f const & o)
		{
			red *= o.red;
			green *= o.green;
			blue *= o.blue;
			alpha *= o.alpha;
		}
		Color4f operator * (Color4f const & o)
		{
			return Color4f(red * o.red, green * o.green, blue * o.blue, alpha * o.alpha);
		}
	};

	
	inline Color4b::Color4b(Color4f const & cf)
	{
		this->set(cf);
	}
	
	inline void Color4b::set(Color4f const & cf)
	{
		r_byte = (uint8_t)(cf.red * 0xff + 0.5f);
		g_byte = (uint8_t)(cf.green * 0xff + 0.5f);
		b_byte = (uint8_t)(cf.blue * 0xff + 0.5f);
		a_byte = (uint8_t)(cf.alpha * 0xff + 0.5f);
	}

	inline float ColorChannelLerp(float from, float to, float value)
	{
		return from + (to - from) * value;
	}
	inline Color4f Color4fLerp(Color4f const & c0, Color4f const & c1, float value)
	{
		return Color4f
		(
		 ColorChannelLerp(c0.red, c1.red, value),
		 ColorChannelLerp(c0.green, c1.green, value),
		 ColorChannelLerp(c0.blue, c1.blue, value),
		 ColorChannelLerp(c0.alpha, c1.alpha, value)
		 );
	}

	typedef Color4b Color;
}
								  
