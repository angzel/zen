/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: LGPL for personnal study or free software.
 */

#pragma once

// depending
#include "zen_macro.h"
#if defined(ZEN_P_LOG_EXCEPTION)
#include "zen_log.h"
#define ZEN_LOG_EXCEPTION 1
#endif

#include <typeinfo>
#include <exception>
#include <string>
#include <stdlib.h>

namespace Zen {
	class Exception : public std::exception
	{
	protected:
		std::string mFile;
		int mLine = 0;
		std::string mWhat;
		int	mError = 0;

	public:
		Exception() throw()
		{
#if defined(ZEN_LOG_EXCEPTION)
			Zen::LogE("Exception");
#endif
		}
		Exception(std::string const & file, int line, std::string const & whatStr, int error = 0) throw()
		: mFile(file)
		, mLine(line)
		, mWhat(whatStr)
		, mError(error)
		{
#if defined(ZEN_LOG_EXCEPTION)
			Zen::LogE("Exception@ %s %d (%s) E=%d", file, line, whatStr, error);
#endif
		}
		virtual char const * what() const throw() override
		{
			return mWhat.data();
		}
		char const * file() const throw()
		{
			return mFile.data();
		}
		int line() const throw()
		{
			return mLine;
		}
		int error() const throw()
		{
			return mError;
		}
	};
}

#define must(cond) do { if(!(cond)) throw ::Zen::Exception(__FILE__,__LINE__,  "Zen::Exception"); } while(0)

#define musts(cond, str) do { if(!(cond)) throw ::Zen::Exception(__FILE__,__LINE__,  str); } while(0)

#define mustsn(cond, str, eno) do { if(!(cond)) throw ::Zen::Exception(__FILE__,__LINE__,  str, eno); } while(0)

#define throws(str) throw ::Zen::Exception(__FILE__,__LINE__, str)

#define throwsn(str, eno) throw ::Zen::Exception(__FILE__,__LINE__, str, eno)

#define readme(...) 

