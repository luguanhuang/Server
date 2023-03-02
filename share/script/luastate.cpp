#include "pch.h"
#include "luastate.h"
#include "luareturn.h"
#include "tolua++.h"
#include "cexception.h"
extern "C"
{
#include "lauxlib.h"
#include <lualib.h>
}


CLuaState::CLuaState()
:m_poLuaState(NULL)
{
}

CLuaState::~CLuaState()
{
}

bool CLuaState::Init()
{
	m_poLuaState = luaL_newstate();
	luaL_openlibs(m_poLuaState);
	return true;
}

void CLuaState::UnInit()
{
	if(m_poLuaState != NULL)
	{
		lua_close(m_poLuaState);
		m_poLuaState = NULL;
	}
}

void CLuaState::ReNew()
{
	UnInit();
	Init();
}

bool CLuaState::LoadFile(const char* pszFilePath)
{
	if(pszFilePath == NULL)
	{
		return false;
	}

	INT32 nLuaErr = luaL_dofile(m_poLuaState, pszFilePath);
	if(nLuaErr == 0)
	{
		return true;
	}

	_HandleLuaError(nLuaErr);
	return false;
}

bool CLuaState::ExecuteString(const char* pszScriptString)
{
	if(pszScriptString == NULL)
	{
		return false;
	}

	INT32 nLuaErr = luaL_dostring(m_poLuaState, pszScriptString);
	if(nLuaErr == 0)
	{
		return true;
	}

	_HandleLuaError(nLuaErr);
	return false;
}

bool CLuaState::CallFunction(CLuaReturn* poReturn, const char* pszTableName, const char* pszFuncName, const char* pszArgFormat, va_list stArgList)
{
	if(pszFuncName == NULL || pszArgFormat == NULL || stArgList == NULL) return false;

	if(pszTableName == NULL)
	{
		lua_getglobal(m_poLuaState, pszFuncName);
		if(lua_isnil(m_poLuaState, -1))
		{
			lua_pop(m_poLuaState, 1);
			LogError("global script function \"%s\" doesn't exist", pszFuncName);
			return false;
		}
	}
	else
	{
		lua_getglobal(m_poLuaState, pszTableName);
		if(lua_isnil(m_poLuaState, -1))
		{
			lua_pop(m_poLuaState, 1);
			LogError("script table \"%s\" doesn't exist", pszTableName);
			return false;
		}

		lua_getfield(m_poLuaState, -1, pszFuncName);
		lua_remove(m_poLuaState, -2);
		if(lua_isnil(m_poLuaState, -1))
		{
			lua_pop(m_poLuaState, 1);
			LogError("script function \"%s.%s\" doesn't exist", pszTableName, pszFuncName);
			return false;
		}
	}

	INT32 nArgCount = 0;
	for(const char* p = pszArgFormat; *p != '\0'; ++nArgCount, ++p)
	{
		switch (*p)
		{
		case 'b' :
			{
				INT32 nParam = va_arg(stArgList, INT32);
				lua_pushboolean(m_poLuaState, nParam);
			}
			break;

		case 'i' :
			{
				INT32 nParam = va_arg(stArgList, INT32);
				lua_pushnumber(m_poLuaState, nParam);
			}
			break;

		case 'n' :
			{
				UINT32 dwParam = va_arg(stArgList, UINT32);
				lua_pushnumber(m_poLuaState, dwParam);
			}
			break;

		case 'd' :
			{
				double dParam = va_arg(stArgList, double);
				lua_pushnumber(m_poLuaState, dParam);
			}
			break;

		case 's' :
			{
				const char* pszParam = va_arg(stArgList, const char*);
				lua_pushstring(m_poLuaState, pszParam);
			}
			break;

		case 'u' :
			{
				void* pObjPtr = va_arg(stArgList, void*);
				const char* pszClassName = va_arg(stArgList, const char*);
				tolua_pushusertype(m_poLuaState, pObjPtr, pszClassName);
			}
			break;

		default:
			LogError("invalid parameter type (%c)", *p);
			break;
		}
	}

	INT32 nResCount = (poReturn == NULL ? 0 : 1);
	INT32 nErrCode = lua_pcall(m_poLuaState, nArgCount, nResCount, 0);     
	_HandleLuaError(nErrCode);

	if(poReturn != NULL && poReturn->m_nType != LUA_RETURN_VOID)
	{
		if(lua_gettop(m_poLuaState) == 0)
		{
			poReturn->m_nType = LUA_RETURN_VOID;
			return true;
		}

		switch(poReturn->m_nType)
		{
		case LUA_RETURN_BOOLEAN:
			{
                poReturn->m_bValue = (lua_toboolean(m_poLuaState, -1) != 0);
                lua_pop(m_poLuaState, 1);
			}
			break;

		case LUA_RETURN_INTEGER:
			{
                poReturn->m_llValue = (INT64)lua_tonumber(m_poLuaState, -1);
                lua_pop(m_poLuaState, 1);
			}
			break;

		case LUA_RETURN_DOUBLE:
			{
				poReturn->m_dValue = lua_tonumber(m_poLuaState, -1);
				lua_pop(m_poLuaState, 1);
			}
			break;

		case LUA_RETURN_STRING:
			{
				poReturn->m_strValue = lua_tostring(m_poLuaState, -1);
				lua_pop(m_poLuaState, 1);
			}
			break;

		default:
			LogError("invalid lua return type: %d", poReturn->m_nType);
			break;
		}
	}
	return true;
}

void CLuaState::_HandleLuaError(INT32 nLuaErr)
{
	if(nLuaErr == 0)
	{
		return;
	}

	const char* pszErrMsg = lua_tostring(m_poLuaState, -1);
	if(pszErrMsg == NULL)
	{
		return;
	}

	std::string strErrMsg = pszErrMsg;
	lua_pop(m_poLuaState, 1);

	throw CException(strErrMsg.c_str());
}