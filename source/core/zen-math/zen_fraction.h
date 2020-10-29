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

#include <iostream>
#include "zen_cast.h"
#include "zen_numerical.h"

namespace Zen {
	class Fraction
	{
	public:
		int num = 0;
		int den = 1;
		Fraction() = default;
		Fraction(Fraction const &) = default;
		Fraction & operator = (Fraction const &) = default;
		
		Fraction(int const & num)
		{
			set(num, 1);
		}
		Fraction(int const & num, int const & den)
		{
			set(num, den);
		}
		void set(int const & num, int const & den = 1)
		{
			this->num = num;
			this->den = den;
		}
		bool operator == (Fraction const & o) const
		{
			if(den == o.den) return num == o.num;
			return num * o.den == o.num * den;
		}
		bool operator != (Fraction const & o) const
		{
			return !(*this == o);
		}

		Fraction operator + (Fraction const & o) const
		{
			if(den == o.den)
			{
				return Fraction(num + o.num, den);
			}
			else
			{
				return Fraction(num * o.den + den * o.num, den * o.den);
			}
		}
		Fraction & operator += (Fraction const & o)
		{
			if(den == o.den)
			{
				num += o.num;
			}
			else
			{
				num = num * o.den + den * o.num;
				den *= o.den;
			}
			return *this;
		}
			// end add

		Fraction operator - (Fraction const & o) const
		{
			if(den == o.den)
			{
				return Fraction(num - o.num, den);
			}
			else
			{
				return Fraction(num * o.den - den * o.num, den * o.den);
			}
		}
		Fraction & operator -= (Fraction const & o)
		{
			if(den == o.den)
			{
				num -= o.num;
			}
			else
			{
				num = num * o.den - den * o.num;
				den *= o.den;
			}
			return *this;
		}
			// end minus

		Fraction operator * (Fraction const & o) const
		{
			return Fraction(num * o.num, den * o.den);
		}
		Fraction & operator *= (Fraction const & o)
		{
			num *= o.num;
			den *= o.den;
			return *this;
		}
		Fraction operator / (Fraction const & o) const
		{
			return Fraction(num * o.den, den * o.num);
		}
		Fraction & operator /= (Fraction const & o)
		{
			num *= o.den;
			den *= o.num;
			return *this;
		}
		Fraction & reduce()
		{
			int gcd = Zen::GetGCD(num, den);
			if(gcd > 1)
			{
				num /= gcd;
				den /= gcd;
			}
			return *this;
		}
		double value() const
		{
			return (double)num/(double)den;
		}
		inline friend Fraction operator + (int const & a, Fraction const & o)
		{
			return Fraction(a) + o;
		}
		inline friend Fraction operator - (int const & a, Fraction const & o)
		{
			return Fraction(a) - o;
		}
		inline friend Fraction operator * (int const & a, Fraction const & o)
		{
			return Fraction(a) * o;
		}
		inline friend Fraction operator / (int const & a, Fraction const & o)
		{
			return Fraction(a) / o;
		}
	};
}

inline bool operator > (Zen::Fraction const & left, Zen::Fraction const & right)
{
	if(left.den == right.den) return left.num > right.num;
	else if(left.num == right.num) return right.den > left.den;
	else return (left.num * right.den) > (right.num * left.den);
}
inline bool operator < (Zen::Fraction const & left, Zen::Fraction const & right)
{
	return right > left;
}
inline bool operator >= (Zen::Fraction const & left, Zen::Fraction const & right)
{
	return !(right > left);
}
inline bool operator <= (Zen::Fraction const & left, Zen::Fraction const & right)
{
	return !(left > right);
}

inline std::ostream & operator << (std::ostream & os, Zen::Fraction const & frac)
{
	os << frac.num << '/' << frac.den;
	return os;
}

