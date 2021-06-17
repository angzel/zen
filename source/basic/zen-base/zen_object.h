/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
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

