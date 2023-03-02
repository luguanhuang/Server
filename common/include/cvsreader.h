#ifndef __CVSREADER_H__
#define __CVSREADER_H__

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "util.h"
#include <algorithm>

typedef unsigned int uint;

using std::string;
using std::vector;
using std::map;

template <typename T>
T type_init();

template<> 
inline int type_init<int>() { return 0; }

template<> 
inline uint type_init<uint>() { return 0; }

template<> 
inline float type_init<float>() { return 0; }

template<> 
inline double type_init<double>() { return 0; }

template<> 
inline std::string type_init<std::string>() { return ""; }

template<> 
inline std::vector<int> type_init<std::vector<int>>() { return std::vector<int>(); }

template<> 
inline std::vector<uint> type_init<std::vector<uint>>() { return std::vector<uint>(); }

template<> 
inline std::vector<float> type_init<std::vector<float>>() { return std::vector<float>(); }

template<> 
inline std::vector<std::string> type_init<std::vector<std::string>>() { return std::vector<std::string>(); }

template <typename T, unsigned N>
struct Sequence
{


	typedef T*       iterator;
	typedef const T* const_iterator;
	Sequence()
	{
		for (unsigned i = 0; i < N; ++i)
		{
			seq[i] = type_init<T>();
		}
	}

	template<typename tn,unsigned osize> 
	void CopyFrom(Sequence<tn,osize> &ref)
	{
		copy(begin(),begin()+N,ref.begin());
	}
	
	const T &operator[](int i) const
	{
		return seq[i];
	}

	T &operator[](int i)
	{
		return seq[i];
	}

	unsigned Size()
	{
		return N;
	}

	bool operator==(const Sequence<T, N>& other) const
	{
		for (unsigned i = 0; i < N; ++i)
		{
			if (seq[i] != other[i])
			{
				return false;
			}
		}
		return true;
	}

	iterator       begin()       { return seq; }
	iterator       end()         { return seq+N; }
	const_iterator begin() const { return seq; }
	const_iterator end()   const { return seq+N; }

	
	T seq[N];

};


class CVSReader
{
public:
	CVSReader() { m_ColNum = 0;}
	virtual ~CVSReader();

	bool LoadFile(const std::string &filename);
	bool LoadBuffer(std::string &buffer, const char *filename="");

	virtual bool OnHeaderLine(char **Fields, int FieldCount) = 0;
	virtual bool OnCommentLine(char **Fields, int FieldCount) = 0;
	virtual bool OnLine(char **Fields, int FieldCount) = 0;

	bool MapColHeader(const char **ColHeader, int HeaderCount, char **Fields, int FieldCount);

	bool Parse(const char *Field, bool &b);
	bool Parse(const char *Field, int &output);
	bool Parse(const char *Field, uint &output);
	bool Parse(const char *Field, float &output);
	bool Parse(const char *Field, double &output);
	bool Parse(const char *Field, long long &output);
	bool Parse(const char *Field, std::string &output);
	
	template<typename T, unsigned N>
	bool Parse(const char *Field, Sequence<T,N> &output, char sep = '=')
	{
		if (Field[0] == 0)
			return true;

		std::vector<std::string> vec = Split(Field, sep);
		if (vec.size() > N)
		{
			return false;
		}

		for (unsigned i = 0; i < vec.size(); ++i)
		{
			if (!Parse(vec[i].c_str(), output.seq[i]))
			{
				return false;
			}
		}
		return true;
	}

	template<typename T>
	bool Parse(const char *Field, std::vector<T> &output, char sep = '|')
	{
		if (Field[0] == 0)
			return true;

		std::vector<std::string> vec = Split(Field, sep);
		for (unsigned i = 0; i < vec.size(); ++i)
		{
			T t;	
			if (!Parse(vec[i].c_str(), t))
			{
				return false; 
			}
			output.push_back(t);
		}
		return true;
	}

	template<typename K, typename V>
	bool Parse(const char *Field, std::map<K,V>&output, char sep1 = '|', char sep2 = '=')
	{
		K k;
		V v;

		if (Field[0] == 0)
			return true;

		std::vector<std::string> vec = Split(Field, sep1);
		for (unsigned i = 0; i < vec.size(); ++i)
		{
			unsigned st = vec[i].find_first_of(sep2);
			if (st == std::string::npos)
			{
				return false;
			}

			vec[i][st] = 0;
			if (!Parse(vec[i].c_str(), k))
			{
				return false;
			}
			if (!Parse(vec[i].c_str()+st+1, v))
			{
				return false;
			}
			output[k] = v;
		}
		return true;
	}


protected:
	int m_ColNum;
	std::vector<int> m_ColMap;
};

#endif