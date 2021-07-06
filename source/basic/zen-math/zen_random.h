/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

#include <cstdint>

namespace Zen
{
	/* Linear Feedback Shift Register(LFSR)  */
	class _RandomLFSR
	{
	protected:
		uint32_t z1, z2, z3, z4;
		// seed limits: z1 > 1, z2 > 7, z3 > 15, z4 > 127
	public:
		static const uint32_t MaxValue = ~(0U);
		
		_RandomLFSR(uint32_t seed = 0)
		{
			reset(seed);
		}
		void reset(uint32_t seed)
		{
			z1 = seed | 0x2U;
			z2 = ((seed >> 4) ^ (seed << 8)) | 0x8U;
			z3 = ((seed >> 6) ^ (seed << 5)) | 0x10U;
			z4 = ((seed >> 8) ^ (seed << 2)) | 0x80U;
		}
		uint32_t operator ()()
		{
			uint32_t tbit;
			tbit = ((z1 << 6) ^ z1) >> 13;
			z1 = ((z1 & 0xFFFFFFFE) << 18) ^ tbit;
			tbit = ((z2 << 2) ^ z2) >> 27;
			z2 = ((z2 & 0xFFFFFFF8) << 2 ) ^ tbit;
			tbit = ((z3 << 13) ^ z3) >> 21;
			z3 = ((z3 & 0xFFFFFFF0) << 7 ) ^ tbit;
			tbit = ((z4 << 3) ^ z4) >> 12;
			z4 = ((z4 & 0xFFFFFF80) << 13) ^ tbit;
			return (z1 ^ z2 ^ z3 ^ z4);
		}
		/*
		 uint32_t next2()
		 {
			uint32_t tbit;
			tbit = ((z1 << 7) ^ z1) >> 14;
			z1 = ((z1 & 0xFFFFFFFE) << 17) ^ tbit;
			tbit = ((z2 << 13) ^ z2) >> 27;
			z2 = ((z2 & 0xFFFFFFF8) << 2 ) ^ tbit;
			tbit = ((z3 << 2) ^ z3) >> 21;
			z3 = ((z3 & 0xFFFFFFF0) << 7 ) ^ tbit;
			tbit = ((z4 << 3) ^ z4) >> 13;
			z4 = ((z4 & 0xFFFFFF80) << 12) ^ tbit;
			return (z1 ^ z2 ^ z3 ^ z4);
		 }
		 */
	};
	/* Well512 */
	class _RandomWell
	{
	protected:
		static const uint32_t Len = 16;
		uint32_t mIndex;
		uint32_t mStates[Len];
	public:
		static const uint32_t MaxValue = ~(0U);
		
		_RandomWell(uint32_t seed = 0)
		{
			reset(seed);
		}
		void reset(uint32_t seed)
		{
			mIndex = 0;
			mStates[0] = seed;
			for(uint32_t i = 1; i < Len; ++i)
				mStates[i] = (seed >> i) ^ ((seed + i) << i);
		}
		/*
		 come from the book : Game Programming Gems 7 by Scott Jacobs.
		 */
		uint32_t operator ()()
		{
			uint32_t a, b, c, d;
			a = mStates[mIndex];
			c = mStates[(mIndex + 13) & 15];
			b = a ^ c ^ (a << 16) ^ (c << 15);
			c = mStates[(mIndex + 9) & 15];
			c ^= (c >> 11);
			a = mStates[mIndex] = b ^ c;
			d = a ^ ((a << 5) & 0xDA442D24U);
			mIndex = (mIndex + 15) & 15;
			a = mStates[mIndex];
			mStates[mIndex] = a ^ b ^ d ^ (a << 2) ^ (b << 18) ^ (c << 28);
			return mStates[mIndex];
		}
	};
	
	/* @ RandomNG
	 tRandRun: raw random function. has operator() and ctor(uint)/ctor()/ctor(copy).
	 VMax: tRandRun will generate the numbers in the range [0, VMax).
	 */
	template<typename TRand = _RandomLFSR, uint32_t VMax = TRand::MaxValue>
	class RandomNG 
	{
	public:
		static const uint32_t MaxValue = VMax;
		typedef TRand Func;
	protected:
		Func mRandFunc;
	public:
		RandomNG()
		{
		}
		RandomNG(Func const &fun)
		: mRandFunc(fun)
		{
		}
		RandomNG(uint32_t seed)
		: mRandFunc(seed)
		{
		}
		void reset(uint32_t seed)
		{
			mRandFunc.reset(seed);
		}
		uint32_t next()
		{
			return mRandFunc();
		}
		uint32_t next(uint32_t max)
		{
			if(max < 2) return 0;
			return mRandFunc() % max;
		}
		float nextf()
		{
			return mRandFunc() / (float)MaxValue;
		}
		virtual uint32_t operator()()
		{
			return next();
		}
	};
	
	typedef RandomNG<> Random;
	
	typedef RandomNG<_RandomLFSR> RandomLFSR;
	
	typedef RandomNG<_RandomWell> RandomWell;
}
