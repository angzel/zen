/*
 Copyright (c) 2013- ClearSky G.
 
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

#include <vector>

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
		void jumpReadPointer(size_t pos)
		{
			m_pos += pos;
		}
		
		void setReadPointer(size_t pos = 0)
		{
			m_pos = pos;
		}
		
		size_t getReadPointer() const
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
