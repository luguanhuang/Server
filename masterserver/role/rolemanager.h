#ifndef _H_Role_Manager_H__
#define _H_Role_Manager_H__

#include "multiindex_map.h"
#include "role.h"

namespace KKSG
{
	class RoleAllInfo;
};

class CRoleSummary;
typedef UINT32 (*FilterRole)(CRoleSummary* me, CRoleSummary* other);

class CRoleManager
{
private:
	CRoleManager();
	~CRoleManager();
	DECLARE_SINGLETON(CRoleManager)
public:
	bool Init();
	void Uninit();
	bool OnTimer();
	CRole* LoginRole(const KKSG::RoleInfoOnMs *pInfo, UINT64 sessionID);
	void   LogoutRole(UINT64 sessionID, UINT32 level);
	void ConvertSession(CRole* pRole, SessionType newSessionID);

	void RemoveBySession(SessionType sessionID);
	UINT32 GetRoleCount() { return m_oRoleDataMap.size(); }

	CRole *GetBySession(SessionType sessionID);
	CRole *GetByRoleID(UINT64 roleID);
	CRole *GetByName(const std::string& name);
	
	void LogoutRoleOnGsLine(UINT32 dwLine, bool isCross = false);

	bool IsOnline(UINT64 roleid);
	void GetRoleState(UINT64 roleid, UINT32& type, UINT32& lastlogout);

	void GetFilterRole(CRoleSummary* role, FilterRole filter, std::vector<UINT64>& onlineroles
		, std::vector<UINT64>& onlineexroles);

	void BroadCast(const CProtocol& ptc, UINT32 line = 0);


	CRole* RecoverRole(const KKSG::RoleInfoOnMs *pInfo, UINT64 sessionID);


	//Foreach (visitor return false means stop)
	template <typename Visitor>
	bool Foreach(Visitor& visitor);
private:
	void AddRole(CRole* role);
	void RemoveRole(CRole* role);
	
	struct SessionIndex
	{
		typedef SessionType key;
		key Key(const CRole *r) { return r->GetSessionID(); }
	};

	struct RoleIDIndex
	{
		typedef UINT64 key;
		key Key(const CRole *r) { return r->GetID(); }
	};

public:
	typedef std::list<CRole*>::iterator role_iterator;
	role_iterator Begin() { return m_oRoleDataMap.begin();}
	role_iterator End()   { return m_oRoleDataMap.end();}

private:
	mutliindex_map_ptr<CRole, SessionIndex, RoleIDIndex> m_oRoleDataMap;
	std::unordered_map<std::string, UINT64> m_oName2IdMap;
	UINT32 m_timer;
};

template <typename Visitor>
bool CRoleManager::Foreach(Visitor& visitor)
{
	for(auto it = m_oRoleDataMap.begin(); it != m_oRoleDataMap.end(); ++it)
	{
		CRole* poRole = *it;
		if(!visitor(poRole)) return false;
	}
	return true;
}

#endif