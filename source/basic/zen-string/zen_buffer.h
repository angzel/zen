/*
 Copyright (c) 2013- MeherTJ G.
All rights reserved. Everybody can use these code freely.
 */

#pragma once

#include <vector>
#include <memory.h>

namespace Zen {
	
	class BufferWriter
	{
	public:
		BufferWriter(std::vector<uint8_t> * ref)
		{
			m_writing = ref;
		}
	
		template<typename TData>
		bool append(TData value)
		{
			auto data = (const char*)&value;
			m_writing->insert(m_writing->end(), data, data+sizeof(value));
			return true;
		}
		bool append(void const * buf, size_t size)
		{
			auto data = (const char*)buf;
			m_writing->insert(m_writing->end(), data, data+size);
			return true;
		}
		template<typename POD_Type>
		bool write(size_t pos, POD_Type value)
		{
			if(pos > m_writing->size()) return false;
			
			auto data = (const char *)&value;
			auto sz = sizeof(value);
			if(pos + sz > m_writing->size()) m_writing->resize(pos + sz);
			::memcpy(m_writing->data() + pos, data, sz);
			return true;
		}
		bool write(size_t pos, void const * buf, size_t size)
		{
			if(pos > m_writing->size()) return false;
			
			auto data = (const char*)buf;
			if(pos + size > m_writing->size()) m_writing->resize(pos + size);
			::memcpy(m_writing->data() + pos, data, size);
			return true;
		}
		
	protected:
		std::vector<uint8_t> * m_writing;
	};


	class BufferReader
	{
	public:
		BufferReader(std::vector<uint8_t> const * ref)
		{
			m_reading = ref;
			m_pos = 0;
		}
		void forward(size_t pos)
		{
			m_pos += pos;
		}
		
		void position(size_t pos)
		{
			m_pos = pos;
		}
		
		size_t position() const
		{
			return m_pos;
		}
		
		template<typename TData>
		bool read(TData & value)
		{
			size_t size = sizeof(value);
			if(m_pos + size > m_reading->size()) return false;
			value = *(TData*)(void*)(m_reading->data() + m_pos);
			m_pos += size;
			return true;
		}
		
		bool read(void * buf, size_t size)
		{
			if(m_pos + size > m_reading->size()) return false;
			::memmove(buf, m_reading->data() + m_pos, size);
			m_pos += size;
			return true;
		}
		
	protected:
		std::vector<uint8_t> const * m_reading;
	private:
		size_t m_pos;
	};
	
	class Buffer : public BufferReader, public BufferWriter
	{
	public:
		Buffer(size_t size = 0)
		: BufferReader(&mData)
		, BufferWriter(&mData)
		{
			mData.resize(size);
		}
		
		Buffer(std::vector<uint8_t> buffer)
		: BufferReader(&mData)
		, BufferWriter(&mData)
		{
			set(buffer);
		}
		
		void set(size_t size)
		{
			mData.resize(size);
		}
		
		void set(std::vector<uint8_t> buffer)
		{
			mData = std::move(buffer);
		}
		
		void resize(size_t size)
		{
			mData.resize(size);
		}

		void clear()
		{
			mData.clear();
		}
		size_t size() const
		{
			return mData.size();
		}
		const uint8_t * bytes() const
		{
			return mData.data();
		}
		std::vector<uint8_t> const & data() const
		{
			return mData;
		}

	protected:
		std::vector<uint8_t> mData;
	};
}
