#ifndef _H_Lua_Return_H__
#define _H_Lua_Return_H__


enum ELuaReturnType
{
	LUA_RETURN_VOID		= 0,
	LUA_RETURN_BOOLEAN,
	LUA_RETURN_INTEGER,
	LUA_RETURN_DOUBLE,
	LUA_RETURN_STRING,
};


class CLuaReturn
{
public:
	CLuaReturn()
		:m_nType(LUA_RETURN_VOID)
		,m_bValue(false)
		,m_llValue(0)
		,m_dValue(0.0f)
	{
	}

	~CLuaReturn()
	{
	}

public:
	ELuaReturnType	m_nType;
	bool			m_bValue;
	INT64			m_llValue;
	double			m_dValue;
	std::string		m_strValue;
};


#endif