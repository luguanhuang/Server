#include "pch.h"
#include "scriptmgr.h"
#include "tolua++.h"
#include "cexception.h"
#include "util.h"
#include "config.h"


INSTANCE_SINGLETON(CScriptMgr)
TOLUA_API int tolua_tolua_open (lua_State* tolua_S);


CScriptMgr::CScriptMgr()
{
}

CScriptMgr::~CScriptMgr()
{
}

bool CScriptMgr::Init(const char* pszFilePath)
{
	if(pszFilePath == NULL)
	{
		return false;
	}

	m_oLuaState.Init();
	tolua_tolua_open(m_oLuaState.GetLuaState());

	m_strPreLoadFile.assign(pszFilePath);
	if(!_PreLoad(m_strPreLoadFile.c_str()))
	{
		return false;
	}

	return true;
}

void CScriptMgr::Uninit()
{
	m_oLuaState.UnInit();

	m_oScriptSet.clear();
	m_oScriptVec.clear();
}

bool CScriptMgr::LoadFile(const char* pszFilePath)
{
	if(pszFilePath == NULL)
	{
		return false;
	}

	if(m_oScriptSet.find(pszFilePath) != m_oScriptSet.end())
	{
		LogError("script file \"%s\" has been loaded", pszFilePath);
		return false;
	}

	if(!_LoadFile(pszFilePath))
	{
		return false;
	}

	m_oScriptSet.insert(pszFilePath);
	m_oScriptVec.push_back(pszFilePath);
	return true;
}

bool CScriptMgr::ReloadAllScripts()
{
	m_oLuaState.ReNew();
	tolua_tolua_open(m_oLuaState.GetLuaState());

	if(!_PreLoad(m_strPreLoadFile.c_str()))
	{
		return false;
	}

	for(CScriptPathVector::size_type i = 0; i < m_oScriptVec.size(); ++i)
	{
		if(!_LoadFile(m_oScriptVec[i].c_str()))
		{
			return false;
		}
	}
	return true;
}

bool CScriptMgr::_LoadFile(const char* pszFilePath)
{
	bool bStatus = false;
	try
	{
		bStatus = m_oLuaState.LoadFile(pszFilePath);
	}
	catch(CException& e)
	{
		LogError("Load script file failed, script: <%s>, error: <%s>", pszFilePath, e.what());
		return false;
	}

	return bStatus;
}

bool CScriptMgr::_PreLoad(const char* pszFilePath)
{
	//Load all files
	std::string strContent = Config::GetConfig()->ReadFileContent(pszFilePath);
	std::vector<std::string> Lines = Split(strContent, "\r\n");
	for (UINT32 i = 0; i < Lines.size(); ++i)
	{
		std::string temp = Trim(Lines[i]);
		if(temp.empty()) continue;

		std::string strLuaFile = Config::GetConfig()->GetFilePath(temp.c_str());
		if(_LoadFile(strLuaFile.c_str()))
		{
			LogInfo("Load lua file: %s success", strLuaFile.c_str());
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool CScriptMgr::CallFunc(CLuaReturn* poReturn, const char* pszTableName, const char* pszFuncName, const char* pszArgFormat, ...)
{
	bool bStatus = false;
	try
	{
		va_list stArgList;
		va_start(stArgList, pszArgFormat);
		bStatus = m_oLuaState.CallFunction(poReturn, pszTableName, pszFuncName, pszArgFormat, stArgList);
		va_end(stArgList);
	}
	catch(CException& e)
	{
		LogError("call function failed, function: <%s.%s>, error: <%s>", pszTableName, pszFuncName, e.what());
		return false;
	}

	return bStatus;
}

bool CScriptMgr::CallGlobalFunc(CLuaReturn* poReturn, const char* pszFuncName, const char* pszArgFormat, ...)
{
	bool bStatus = false;
	try
	{
		va_list stArgList;
		va_start(stArgList, pszArgFormat);
		bStatus = m_oLuaState.CallFunction(poReturn, NULL, pszFuncName, pszArgFormat, stArgList);
		va_end(stArgList);
	}
	catch(CException& e)
	{
		LogError("call function failed, function: <%s>, error: <%s>", pszFuncName, e.what());
		return false;
	}

	return bStatus;
}

bool CScriptMgr::ExecuteFile(const char* pszScriptFile)
{
	if(pszScriptFile == NULL) return false;

	std::string strContent = Config::GetConfig()->ReadFileContent(pszScriptFile);
	if(strContent.empty()) return true;

	return ExecuteString(strContent.c_str());
}

bool CScriptMgr::ExecuteString(const char* pszScriptString)
{
	bool bStatus = false;
	try
	{
		bStatus = m_oLuaState.ExecuteString(pszScriptString);
	}
	catch(CException& e)
	{
		LogError("execute string failed, string: <%s>, error: <%s>", pszScriptString, e.what());
		return false;
	}

	return bStatus;
}