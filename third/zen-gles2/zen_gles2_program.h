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

#include "zen_gles2_enum.h"
#include "zen_log.h"

namespace Zen { namespace GL {
	template<GLenum tType>
	class Shader
	{
	private:
		Shader(Shader const &);

		void operator = (Shader const &);

		static GLenum const Type = tType;
	public:

	public:
		inline Shader();

		inline ~Shader();

		inline void create();

		inline void release();

		inline GLuint getObject() const;

		inline void compile(const char * str);

		inline std::string getStatus();

	protected:
		GLuint mData;
	};

	typedef Shader<GL_VERTEX_SHADER> VertexShader;

	typedef Shader<GL_FRAGMENT_SHADER> FragmentShader;;
} }


namespace Zen { namespace GL { 
	class Program
	{
	private:
		Program(Program const &);
		
		void operator = (Program const &);
		
	public:
		static void ActiveNone();
		
	public:
		Program();
		
		virtual ~Program();
		
		void createProgram();
		
		void releaseProgram();
		
		void makeAttachAndLink(char const * vertexSrc, char const * fragmentSrc);
		
		void makeAttachAndLink(VertexShader const &, FragmentShader const &);
		
		/* @ isValidate 
		 This is just for debug, it's so slowly.
		 */
		int getValidateStatus() const;
		
		int getUniformCount() const;
		
		int getUniformMaxLength() const;
		
		int getAttributeCount() const;
		
		int getAttributeMaxLength() const;
		
		
		GLint getUniformLocation(char const * name) const;
		
		GLint getAttributeLocation(char const * name) const;
		
		GLuint getObject() const;
		
		std::string getStateInfo();
		
	protected:
		GLuint mData;
	};
} }


/// class Shader
namespace Zen { namespace GL {

	template<GLenum tType>
	inline Shader<tType>::Shader()
	{
		mData = 0;
	}
	template<GLenum tType>
	inline void Shader<tType>::release()
	{
		if(mData == 0) return;
		glDeleteShader(mData);
		mData = 0;
	}
	template<GLenum tType>
	inline void Shader<tType>::create()
	{
		if(mData == 0)
		{
			mData = glCreateShader(Type);
		}
#if ZEN_DEBUG
		mustsn(mData != 0, "Failed to create shader", (int)glGetError());
#endif
	}
	template<GLenum tType>
	inline GLuint Shader<tType>::getObject() const
	{
		return mData;
	}
	template<GLenum tType>
	inline Shader<tType>::~Shader()
	{
		if(mData != 0) glDeleteShader(mData);
	}
	template<GLenum tType>
	inline void Shader<tType>::compile(const char * str)
	{
		create();
		glShaderSource(mData, (GLsizei)1, &str, nullptr);
		glCompileShader(mData);
		GLint compiled;
		glGetShaderiv(mData, GL_COMPILE_STATUS, &compiled );
#if ZEN_DEBUG
		if(!compiled)
		{
			Zen::LogD("%s", this->getStatus().data());
		}
#endif
		musts(compiled, "failed to compile shader");
	}
	template<GLenum tType>
	inline std::string Shader<tType>::getStatus()
	{
		GLint infoLen = 0;
		glGetShaderiv (mData, GL_INFO_LOG_LENGTH, &infoLen );
		if ( infoLen > 1 )
		{
			std::string status;
			status.resize(infoLen);
			glGetShaderInfoLog(mData, infoLen, NULL, (GLchar*)status.data());
			return status;
		}
		else
		{
			return "";
		}
	}

} }

// class Program
namespace Zen { namespace GL {
	
	inline static void ActiveNone()
	{
		glUseProgram(0);
	}
	inline void Program::makeAttachAndLink(char const * vertexSrc, char const * fragmentSrc)
	{
		VertexShader vertex;
		FragmentShader fragment;
		vertex.compile(vertexSrc);
		fragment.compile(fragmentSrc);
		makeAttachAndLink(vertex, fragment);
	}
	inline void Program::makeAttachAndLink(VertexShader const & vertex, FragmentShader const & fragment)
	{
		createProgram();
		glAttachShader(mData, vertex.getObject());
		glAttachShader(mData, fragment.getObject());
		glLinkProgram(mData);
#if ZEN_DEBUG
		GLint linked;
		glGetProgramiv(mData, GL_LINK_STATUS, &linked);
		musts(linked, "failed to link GL-program");
		if(!linked)
		{
			Zen::LogD("%s", this->getStateInfo().data());
		}
#endif
	}
	inline std::string Program::getStateInfo()
	{
		GLint infoLen = 0; 
		glGetProgramiv(mData, GL_INFO_LOG_LENGTH, &infoLen); 
		
		if(infoLen > 1) 
		{
			std::string status;
			status.resize(infoLen);
			glGetProgramInfoLog(mData, infoLen, NULL, (GLchar*)status.data());
			return status;
		}
		return "";
	}
	inline Program::Program()
	{
		mData = 0;
	}
	inline Program::~Program()
	{
		if(mData != 0) glDeleteProgram(mData);
	}
	inline void Program::createProgram()
	{
		if(mData != 0) releaseProgram();

		mData = glCreateProgram();
#if ZEN_DEBUG
		mustsn(mData != 0,  "failed to create program", (int)glGetError());
#endif
	}
	inline void Program::releaseProgram()
	{
		if(mData == 0) return;
		glDeleteProgram(mData);
		mData = 0;
	}
	inline int Program::getValidateStatus() const
	{
		GLint status;
		glValidateProgram(mData);
		glGetProgramiv(mData, GL_VALIDATE_STATUS, &status);
		return status;
	}
	inline int Program::getUniformCount() const
	{
		GLint numUniforms;     
		glGetProgramiv(mData, GL_ACTIVE_UNIFORMS, &numUniforms);
		return numUniforms;
	}
	inline int Program::getUniformMaxLength() const
	{
		GLint length;
		glGetProgramiv(mData, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
		return length;
	}
	inline int Program::getAttributeCount() const
	{
		GLint value;     
		glGetProgramiv(mData, GL_ACTIVE_ATTRIBUTES, &value);
		return value;
	}
	inline int Program::getAttributeMaxLength() const
	{
		GLint value;     
		glGetProgramiv(mData, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &value);
		return value;
	}
	inline GLint Program::getUniformLocation(char const * name) const
	{
#if ZEN_DEBUG
		auto ret = glGetUniformLocation(mData, name);
		musts(ret != -1, "invalid uniform location");
		return (GLint)ret;
#else
		return glGetUniformLocation(mData, name);
#endif
	}
	inline GLint Program::getAttributeLocation(char const * name) const
	{
#if ZEN_DEBUG
		auto ret = glGetAttribLocation(mData, name);
		musts(ret != -1, "invalid attrib location");
		return (GLint)ret;
#else
		return glGetAttribLocation(mData, name);
#endif
	}
	inline GLuint Program::getObject() const
	{
		return mData;
	}
} }
