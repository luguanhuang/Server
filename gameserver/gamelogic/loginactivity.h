#ifndef __ACTIVITY_LOGIN_H_
#define __ACTIVITY_LOGIN_H_
#include "table/LoginActivity.h"


class Role;
class LoginActivityMgr
{
	DECLARE_SINGLETON(LoginActivityMgr);
public:
	LoginActivityMgr();
	~LoginActivityMgr();
public:
	bool Init();
	void Uninit();
	void ClearFile();
	bool LoadFile();
	bool OnStart(UINT32 nTime);
	void ControlActivity(bool bOpen);
	bool OnClose(UINT32 nTime);
	void OnTriger(Role *pRole,bool bLogin);
	void GiveAward(Role *pRole);
	void SetWolrdLevel(UINT32 nWorldLevel,bool bSave = false);
	void Pack(Role *pRole, KKSG::LoginActivityRes &res);
private:
	std::map<UINT32,UINT32> m_mapHandle;
	UINT32 m_nTime;
	LoginActivity m_Table;
	std::multimap<UINT32, LoginActivity::RowData *> m_mmapActivity;
	UINT32 m_nWorldLevel;
};

#endif