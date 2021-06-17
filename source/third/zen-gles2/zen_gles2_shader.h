/*
 Copyright (c) 2013 MeherTJ G. All rights reserved.
 License: Everybody can use these code freely.
 */

#pragma once

#include "zen_gles2_enum.h"
#include "zen_log.h"

namespace Zen { namespace GL {
	template<GLenum tType>
	class tShader
	{
	private:
		tShader(tShader const &);
		
		void operator = (tShader const &);
		
		static GLenum const Type = tType;
	public:
		
	public:
		inline tShader();
		
		inline virtual ~tShader();
		
		inline GLuint getID() const;
		
		inline void compile(const char * str);
		
		inline std::string getStatus();
		
	protected:
		GLuint mData;
	};
	
	typedef tShader<GL_VERTEX_SHADER> VertexShader;
	
	typedef tShader<GL_FRAGMENT_SHADER> FragmentShader;;
} }


namespace Zen { namespace GL { 
	class ShaderProgram
	{
	private:
		ShaderProgram(ShaderProgram const &);
		
		void operator = (ShaderProgram const &);
		
	public:
		ShaderProgram();
		
		virtual ~ShaderProgram();
		
		void makeAttachAndLink(char const * vertexSrc, char const * fragmentSrc);
		
		void makeAttachAndLink(VertexShader const &, FragmentShader const &);
		
		/* @ isValidate 
		 This is just for debug, it's so slowly.
		 */
		int getDebugValidateStatus() const;
		
		int getUniformCount() const;
		
		int getUniformMaxLength() const;
		
		int getAttributeCount() const;
		
		int getAttributeMaxLength() const;

		GLint getUniformLocation(char const * name) const;
		
		GLint getAttributeLocation(char const * name) const;
		
		GLuint getID() const;
		
		std::string getStateInfo();
		
	protected:
		GLuint mData;
	};
} }


	/// class tShader
namespace Zen { namespace GL {
	
	template<GLenum tType>
	inline tShader<tType>::tShader()
	{
		mData = glCreateShader(Type);
#if ZEN_DEBUG
		mustsn(mData != 0, "Failed to create shader", (int)glGetError());
#endif
	}
	template<GLenum tType>
	inline GLuint tShader<tType>::getID() const
	{
		return mData;
	}
	template<GLenum tType>
	inline tShader<tType>::~tShader()
	{
		if(mData != 0) glDeleteShader(mData);
	}
	template<GLenum tType>
	inline void tShader<tType>::compile(const char * str)
	{
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
	inline std::string tShader<tType>::getStatus()
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

	// class ShaderProgram
namespace Zen { namespace GL {
	
	inline static void ActiveNone()
	{
		glUseProgram(0);
	}
	inline void ShaderProgram::makeAttachAndLink(char const * vertexSrc, char const * fragmentSrc)
	{
		VertexShader vertex;
		FragmentShader fragment;
		vertex.compile(vertexSrc);
		fragment.compile(fragmentSrc);
		makeAttachAndLink(vertex, fragment);
	}
	inline void ShaderProgram::makeAttachAndLink(VertexShader const & vertex, FragmentShader const & fragment)
	{
		glAttachShader(mData, vertex.getID());
		glAttachShader(mData, fragment.getID());
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
	inline std::string ShaderProgram::getStateInfo()
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
	inline ShaderProgram::ShaderProgram()
	{
		mData = glCreateProgram();
#if ZEN_DEBUG
		mustsn(mData != 0,  "failed to create program", (int)glGetError());
#endif
	}
	inline ShaderProgram::~ShaderProgram()
	{
		if(mData != 0) glDeleteProgram(mData);
	}
	inline int ShaderProgram::getDebugValidateStatus() const
	{
		GLint status;
		glValidateProgram(mData);
		glGetProgramiv(mData, GL_VALIDATE_STATUS, &status);
		return status;
	}
	inline int ShaderProgram::getUniformCount() const
	{
		GLint numUniforms;     
		glGetProgramiv(mData, GL_ACTIVE_UNIFORMS, &numUniforms);
		return numUniforms;
	}
	inline int ShaderProgram::getUniformMaxLength() const
	{
		GLint length;
		glGetProgramiv(mData, GL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
		return length;
	}
	inline int ShaderProgram::getAttributeCount() const
	{
		GLint value;     
		glGetProgramiv(mData, GL_ACTIVE_ATTRIBUTES, &value);
		return value;
	}
	inline int ShaderProgram::getAttributeMaxLength() const
	{
		GLint value;     
		glGetProgramiv(mData, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &value);
		return value;
	}
	inline GLint ShaderProgram::getUniformLocation(char const * name) const
	{
#if ZEN_DEBUG
		auto ret = glGetUniformLocation(mData, name);
		musts(ret != -1, "invalid uniform location");
		return (GLint)ret;
#else
		return glGetUniformLocation(mData, name);
#endif
	}
	inline GLint ShaderProgram::getAttributeLocation(char const * name) const
	{
#if ZEN_DEBUG
		auto ret = glGetAttribLocation(mData, name);
		musts(ret != -1, "invalid attrib location");
		return (GLint)ret;
#else
		return glGetAttribLocation(mData, name);
#endif
	}
	inline GLuint ShaderProgram::getID() const
	{
		return mData;
	}
} }
