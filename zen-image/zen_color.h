/*
 Copyright (c) 2013 ClearSky G.
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <cstdint>

namespace Zen 
{
	class Color4;
	class Color4f;
	class ColorRBG;
	
	/**
	 enum usual color's name in RGB format.
	 */
	enum class EColor : uint32_t
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


	class Color3
	{
	public:
		uint8_t r_byte, g_byte, b_byte;
	public:
		Color3()
		{
			r_byte = g_byte = b_byte = 0;
		}
		Color3(EColor rgb)
		{
			this->set((uint32_t)rgb);
		}
		Color3(uint32_t rgb)
		{
			this->set(rgb);
		}
		Color3(uint8_t r, uint8_t g, uint8_t b)
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
		
		bool operator == (Color3 const & o) const
		{
			return r_byte == o.r_byte && g_byte == o.g_byte && b_byte == o.b_byte;
		}
		
		bool operator != (Color3 const & o) const
		{
			return !(*this == o);
		}
	};


	/* C: Color4
	 - 32bits Color4 in RGBA format, 0xRRGGBBAA.
	 */


	class Color4
	{
	public:
		uint8_t r_byte, g_byte, b_byte, a_byte;
	public:
		Color4(EColor e)
		{
			set(Color3(e));
		}
		/* 0xRRGGBBAA */
		Color4(uint32_t rgba = 0x0FF)
		{
			setRGBA(rgba);
		}
		
		Color4(Color3 const & rgb, uint8_t a = 0xFF)
		{
			set(rgb, a);
		}

		Color4(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
		{
			set(r, g, b, a);
		}

		inline Color4(Color4f const & cf);

		inline void set(Color4f const &);

		void set(Color3 const & rgb, uint8_t a = 0xFF)
		{
			r_byte = rgb.r_byte;
			g_byte = rgb.g_byte;
			b_byte = rgb.b_byte;
			a_byte = a;
		}

		void set(EColor value, uint8_t a = 0xFF)
		{
			this->set(Color3(value), a);
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
		
		bool operator == (Color4 const & o)
		{
			return this->getARGB() == o.getARGB();
		}
		
		bool operator != (Color4 const & o)
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

		operator Color3 ()
		{
			return Color3(r_byte, g_byte, b_byte);
		}
	};
	

	 
	/** Color4f
	 -- Color4 by 4 float
	 */
	class Color4f
	{
	public:
		float red, green, blue, alpha;
		
	public:
		Color4f(EColor e)
		{
			set(Color4(e));
		}
		
		Color4f()
		{
			red = green = blue = 0.f;
			alpha = 1.f;
		}

		Color4f(Color4 const & color)
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
		
		void set(Color4 const & color)
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

	
	inline Color4::Color4(Color4f const & cf)
	{
		this->set(cf);
	}
	
	inline void Color4::set(Color4f const & cf)
	{
		r_byte = (uint8_t)(cf.red * 0xff + 0.5f);
		g_byte = (uint8_t)(cf.green * 0xff + 0.5f);
		b_byte = (uint8_t)(cf.blue * 0xff + 0.5f);
		a_byte = (uint8_t)(cf.alpha * 0xff + 0.5f);
	}
	
	/** ColorGradient
	 -- ColorGradient color function by value 0.0 - 1.0
	 */
	class ColorGradient
	{
	protected:
		Color4f mC0;
		
		Color4f mC1;
	public:
		ColorGradient() = default;
		
		ColorGradient(Color4f const & c0, Color4f const & c1)
		{
			set(c0, c1);
		}
		
		Color4f get0() const
		{
			return mC0;
		}
		
		Color4f get1() const
		{
			return mC1;
		}
		
		void set(Color4f const & c0, Color4f const & c1)
		{
			mC0 = c0;
			mC1 = c1;
		}
		
		Color4f get(float value)
		{
			return Color4f
			(
			 Grey(mC0.red, mC1.red, value),
			 Grey(mC0.green, mC1.green, value),
			 Grey(mC0.blue, mC1.blue, value),
			 Grey(mC0.alpha, mC1.alpha, value)
			 );
		}
		
		Color4f operator() (float value)
		{
			return this->get(value);
		}
		
		static float Grey(float from ,float to, float rat)
		{
			return from + (to - from) * rat;
		}
	};
	
	typedef Color4 Color;
}
								  
