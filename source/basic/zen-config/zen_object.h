/*
 Copyright (c) 2013 MeherTJ G.
 
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
#include <cassert>
#include <memory>

namespace Zen {

	class Data
	{
	public:
		Data() = default;
	};
	
	
	class Object
	{
	public:
		Object() = default;
		virtual ~Object() = default;
	};
	
	class Uncopyable : public Object
	{
	public:
		Uncopyable() = default;
		
		Uncopyable(Uncopyable const &) = delete;

		void operator = (Uncopyable const &) = delete;
	};

	template<typename _Type>
	std::shared_ptr<_Type> MakeShared(_Type * ptr)
	{
		return std::shared_ptr<_Type>(ptr);
	}
}

#define PUBLIC_MEMBER(type, name, get_method, set_method) \
public: type const & get_method() const { return name; } \
public: type & get_method() { return name; } \
public: void set_method(type const & v) { this->name = v; } \
protected: type name

#define PUBLIC_MEMBER_COPY(type, name, get_method, set_method) \
public: type get_method() const { return name; } \
public: void set_method(type v) { this->name = v; } \
protected: type name

#define PUBLIC_MEMBER_CONST(type, name, get_method, set_method) \
public: type const & get_method() const { return name; } \
public: void set_method(type const & v) { this->name = v; } \
protected: type name

#define PUBLIC_MEMBER_GET(type, name, get_method) \
public: type const & get_method() const { return name; } \
public: type & get_method() { return name; } \
protected: type name

#define PUBLIC_MEMBER_COPY_GET(type, name, get_method) \
public: type get_method() const { return name; } \
protected: type name

#define PUBLIC_MEMBER_CONST_GET(type, name, get_method) \
public: type const & get_method() const { return name; } \
protected: type name

