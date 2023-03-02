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



	// ����lua���������������ڲ����óɹ��󷵻�true�����򷵻�false
	// poReturn:	����ֵ���ɹ�ʱʱ��Ч���޷���ֵ����NULL
	// pszArgFormat:ָ���������͸�ʽ��Ŀǰֻ֧�����¼��֣�
	//               b - bool���ͣ�0/1
	//               i - ����(INT32)
	//               n - ����(UINT32)
	//               d - double
	//               s - const char*
	//               u - �û��Զ�������(����Ҫ���⴫��������)
	//����:			CallFunc(NULL, NULL, "OnPlayerLogin", "ui", poPlayer, "CPlayer", 100);

	//���þֲ�����
	bool CallFunc(CLuaReturn* poReturn, const char* pszTableName, const char* pszFuncName, const char* pszArgFormat, ...);


	//����ȫ�ֺ���
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