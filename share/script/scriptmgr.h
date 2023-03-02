#ifndef _H_ScriptMgr_H__
#define _H_ScriptMgr_H__

#include "luastate.h"
#include "luareturn.h"


class CScriptMgr
{
    CScriptMgr();
    ~CScriptMgr();
    DECLARE_SINGLETON(CScriptMgr);

public:
    bool Init(const char* pszFilePath);
    void Uninit();


    bool LoadFile(const char* pszFilePath);
    bool ReloadAllScripts();
	CLuaState GetLuaState() { return m_oLuaState; }



	// 调用lua函数，当函数存在并调用成功后返回true，否则返回false
	// poReturn:	返回值，成功时时有效，无返回值传入NULL
	// pszArgFormat:指定参数类型格式，目前只支持如下几种：
	//               b - bool类型，0/1
	//               i - 数字(INT32)
	//               n - 数字(UINT32)
	//               d - double
	//               s - const char*
	//               u - 用户自定义类型(还需要额外传入类型名)
	//例子:			CallFunc(NULL, NULL, "OnPlayerLogin", "ui", poPlayer, "CPlayer", 100);

	//调用局部函数
	bool CallFunc(CLuaReturn* poReturn, const char* pszTableName, const char* pszFuncName, const char* pszArgFormat, ...);


	//调用全局函数
	bool CallGlobalFunc(CLuaReturn* poReturn, const char* pszFuncName, const char* pszArgFormat, ...);


    bool ExecuteFile(const char* pszScriptFile);
    bool ExecuteString(const char* pszScriptString);
private:
	bool _LoadFile(const char* pszFilePath);
	bool _PreLoad(const char* pszFilePath);

private:
    typedef std::set<std::string>		CScriptPathSet;
	typedef std::vector<std::string>	CScriptPathVector;

	CLuaState			m_oLuaState;
    CScriptPathSet		m_oScriptSet;
    CScriptPathVector	m_oScriptVec;
	std::string			m_strPreLoadFile;
};

#endif