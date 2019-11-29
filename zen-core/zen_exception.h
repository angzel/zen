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

// depending
#include "zen_os.h"
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
		virtual char const * what() const throw() 
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

