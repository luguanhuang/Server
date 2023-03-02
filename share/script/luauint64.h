#ifndef _H_Lua_Uint64_H__
#define _H_Lua_Uint64_H__


class CUint64
{
public:
	CUint64()
		:m_qwVal(0)
	{
	}

	CUint64(UINT64 qwVal)
		:m_qwVal(qwVal)
	{
	}

	~CUint64()
	{
	}

	operator UINT64()
	{
		return m_qwVal;
	}

	std::string tostr()
	{
		std::stringstream ss;
		ss << m_qwVal;
		return ss.str();
	}

	void set(const std::string& str)
	{
		std::stringstream ss(str);
		ss >> m_qwVal;
	}

private:
	UINT64	m_qwVal;
};


#endif