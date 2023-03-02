/************************************************************************
filename:			ByteStream.h
purpose:			byte stream
author:				liyang
createdL:			2010.04.28

vector<char> 作为容器效率最高	比std::string, 直接new都要高

*************************************************************************/

#ifndef _Byte_Stream_H__
#define _Byte_Stream_H__

#include <vector>
#include <list>
#include <deque>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <string>
#include <algorithm>
#include <string.h>
#include "define.h"

struct ByteStreamError
{
	ByteStreamError()
	{
		totalSize = 0;
		errorPosition = 0;
		expectSize = 0;
	}

	int  totalSize;
	int  errorPosition;
	int  expectSize;
};


class ByteStream
{
public:
	ByteStream()
		:m_read_pos(0)
	{
		m_storage.reserve(128);
	}

	ByteStream(const void* pData, size_t nLen)
		:m_read_pos(0)
	{
		Assign(pData, nLen);
	}

	ByteStream(const ByteStream& rht)
		:m_storage(rht.m_storage)
		,m_read_pos(0)
	{
	}

	~ByteStream()
	{
	}


	ByteStream& operator = (const ByteStream& rht)
	{
		Assign(rht.Data(), rht.Size());
		return *this;
	}

	const char* Data() const
	{
		return &m_storage[0];
	}

	UINT32 Size() const
	{
		return m_storage.size();
	}

	void Assign(const void* pBuf, size_t size)
	{
		Clear(); 
		Write(pBuf, size);
	}

	void Swap(ByteStream& rht)
	{
		m_storage.swap(rht.m_storage);
		std::swap(m_read_pos, rht.m_read_pos);
	}

	void Clear()
	{
		m_storage.clear();
		m_read_pos = 0;
	}

	bool Empty() const
	{
		return m_read_pos == Size();
	}


	ByteStream& operator << (const ByteStream& stream)
	{
		*this << (UINT32) stream.Size();
		Write(stream.Data(), stream.Size()); 
		return *this;
	}

	const ByteStream& operator >> (ByteStream& stream) const
	{
		UINT32 size = 0;
		*this >> size;
		stream.Clear();

		if(size == 0)
		{
			return *this;
		}

		if(m_read_pos + size > Size())
		{
			ByteStreamError error;
			error.errorPosition = m_read_pos;
			error.totalSize = Size();
			error.expectSize = size;
			throw error;

			return *this;
		}

		stream.Write(&m_storage[m_read_pos], size);
		m_read_pos += size; 
		return *this;
	}


	//set data directly
	void Set(int destpos, const void* data, size_t len)
	{ 
		if(destpos + len > Size())
		{
			return;
		}

		memcpy(&m_storage[destpos], data, len);
	}

	inline void Write(const void* pBuff, size_t size)
	{
		m_storage.insert(m_storage.end(), (char*)pBuff, (char*)pBuff + size);
	}

	inline bool Read(void* pBuff, size_t size) const
	{
		if( m_read_pos + size > Size())
		{
			ByteStreamError error;
			error.errorPosition = m_read_pos;
			error.totalSize = Size();
			error.expectSize = size;
			throw error;
			return false;
		}

		memcpy(pBuff, &m_storage[m_read_pos], size);
		m_read_pos += size;
		return true;
	}


	//common type's push and pop
	template<class T>
	ByteStream& PushBack(const T& val)
	{
		Write(&val, sizeof(T));
		return *this;		
	}

	template<class T>
	const ByteStream& PopFront(T& val) const
	{
		if(m_read_pos + sizeof(T) > Size())
		{
			ByteStreamError error;
			error.errorPosition = m_read_pos;
			error.totalSize = Size();
			error.expectSize = sizeof(T);
			throw error;

			return *this;
		}

		val = *((T*)&m_storage[m_read_pos]);	//memcpy(&val, &m_storage[m_read_pos], sizeof(T));
		m_read_pos += sizeof(T); 
		return *this;
	}


	//basic type support
	inline ByteStream& operator << (INT8 val)   { return PushBack(val); }
	inline ByteStream& operator << (UINT8 val)  { return PushBack(val); }
	inline ByteStream& operator << (INT16 val)  { return PushBack(val); }
	inline ByteStream& operator << (UINT16 val) { return PushBack(val); }
	inline ByteStream& operator << (INT32 val)  { return PushBack(val); }
	inline ByteStream& operator << (UINT32 val) { return PushBack(val); }
	inline ByteStream& operator << (INT64 val)  { return PushBack(val); }
	inline ByteStream& operator << (UINT64 val) { return PushBack(val); }
	inline ByteStream& operator << (float val)  { return PushBack(val); }
	inline ByteStream& operator << (double val) { return PushBack(val); }

	inline const ByteStream& operator >> (INT8& val) const   { return PopFront(val); }
	inline const ByteStream& operator >> (UINT8& val) const  { return PopFront(val); }
	inline const ByteStream& operator >> (INT16& val) const  { return PopFront(val); }
	inline const ByteStream& operator >> (UINT16& val) const { return PopFront(val); }
	inline const ByteStream& operator >> (INT32& val) const  { return PopFront(val); }
	inline const ByteStream& operator >> (UINT32& val) const { return PopFront(val); }
	inline const ByteStream& operator >> (INT64& val) const  { return PopFront(val); }
	inline const ByteStream& operator >> (UINT64& val) const { return PopFront(val); }
	inline const ByteStream& operator >> (float& val) const  { return PopFront(val); }
	inline const ByteStream& operator >> (double& val) const { return PopFront(val); }

	//std::string
	ByteStream& operator << (const std::string& val)
	{
		UINT32 size = val.size();
		*this << size;

		if(size > 0)
		{
			Write(val.c_str(), size);
		}
		return *this;
	}

	const ByteStream& operator >> (std::string& val) const
	{
		UINT32 size = 0;
		*this >> size;
		val.clear();

		if(size > 0)
		{
			val.assign(&m_storage[m_read_pos], size);
			m_read_pos += size;
		}
		return *this;
	}


	//common stl container input
	template <class _InIt>
	void push_container(size_t size, _InIt _First, _InIt _Last)
	{
		*this << (UINT32)size;
		for(_InIt it = _First; it != _Last; ++it)
		{
			*this << *it;
		}
	}

	//vector
	template <class type>
	ByteStream& operator << (const std::vector<type>& val)
	{
		push_container(val.size(), val.begin(), val.end());
		return *this;
	}

	template <class type>
	const ByteStream& operator >> (std::vector<type>& val) const
	{
		UINT32 size = 0;
		*this >> size;
		val.resize(size);

		for(size_t i = 0; i < size; ++i)
		{
			(*this) >> val[i];
		}
		return *this;
	}

	//list
	template <class type>
	ByteStream& operator << (const std::list<type>& val)
	{
		push_container(val.size(), val.begin(), val.end());
		return *this;
	}

	template <class type>
	const ByteStream& operator >> (std::list<type>& val) const
	{
		UINT32 size = 0;
		*this >> size;
		val.clear();

		for(size_t i = 0; i < size; ++i)
		{
			type temp;
			*this >> temp;
			val.push_back(temp);
		}
		return *this;
	}

	//dequeue
	template <class type>
	ByteStream& operator << (const std::deque<type>& val)
	{
		push_container(val.size(), val.begin(), val.end());
		return *this;
	}

	template <class type>
	const ByteStream& operator >> (std::deque<type>& val) const
	{
		UINT32 size = 0;
		*this >> size;
		val.clear();

		for(size_t i = 0; i < size; ++i)
		{
			type temp;
			*this >> temp;
			val.push_back(temp);
		}
		return *this;
	}

	//std::pair
	template <class First, class Second>
	ByteStream& operator << (const std::pair<First, Second>& val)
	{
		*this << val.first;
		*this << val.second;
		return *this;
	}

	template <class First, class Second>
	const ByteStream& operator >> (std::pair<First, Second>& val) const
	{
		*this >> val.first;
		*this >> val.second;
		return *this;
	}


	//map
	template <class key, class value>
	ByteStream& operator << (const std::map<key, value>& val)
	{
		push_container(val.size(), val.begin(), val.end());
		return *this;
	}

	template <class key, class value>
	const ByteStream& operator >> (std::map<key, value>& val) const
	{
		UINT32 size = 0;
		*this >> size;
		val.clear();

		for(size_t i = 0; i < size; ++i)
		{
			std::pair<key, value> temp;
			*this >> temp;
			val.insert(temp);
		}
		return *this;
	}


	//set
	template <class type>
	ByteStream& operator << (const std::set<type>& val)
	{
		push_container(val.size(), val.begin(), val.end());
		return *this;
	}

	template <class type>
	const ByteStream& operator >> (std::set<type>& val) const
	{
		UINT32 size = 0;
		*this >> size;
		val.clear();

		for(size_t i = 0; i < size; ++i)
		{
			type temp;
			*this >> temp;
			val.insert(temp);
		}
		return *this;
	}

protected:
	typedef std::vector<char>	Container;
	Container		m_storage;
	mutable size_t	m_read_pos;
};


#endif
