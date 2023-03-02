#ifndef _Lua_State_H__
#define _Lua_State_H__

struct lua_State;
class CLuaReturn;


class CLuaState
{
public:
	CLuaState();
	~CLuaState();

	bool Init();
	void UnInit();
	void ReNew();

	//throw exception when error occurs
	bool LoadFile(const char* pszFilePath);
    bool ExecuteString(const char* pszScriptString);
	bool CallFunction(CLuaReturn* poReturn, const char* pszTableName, const char* pszFuncName, const char* pszArgFormat, va_list stArgList);

	lua_State* GetLuaState() { return m_poLuaState; }
private:
	void _HandleLuaError(INT32 nLuaErr);

private:
	lua_State*	m_poLuaState;
};


#endif