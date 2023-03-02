#ifndef _H_Role_Manager_H__
#define _H_Role_Manager_H__

#include "multiindex_map.h"
#include "role.h"

namespace KKSG
{
	class RoleAllInfo;
};


class CRoleManager
{
private:
	CRoleManager();
	~CRoleManager();
	DECLARE_SINGLETON(CRoleManager)
public:
	bool Init();
	void Uninit();
	void RecoverFromShm();

	CRole* ConvertSession(SessionType oldSession, SessionType newSession);
	CRole* LoginRole(const KKSG::RoleAllInfo *pInfo, const KKSG::ClientInfo& roCliConf, UINT64 sessionID);
	void   LogoutRole(UINT64 sessionID);

	void RemoveBySession(SessionType sessionID);
	UINT32 GetRoleCount() { return m_oRoleDataMap.size(); }
	UINT32 GetRoleCount(UINT32 line, bool isCross = false);

	CRole *GetBySession(SessionType sessionID);
	CRole *GetByRoleID(UINT64 roleID);
	CRole *GetByName(const std::string& name);
	
	void KickPlayerFromGate(int GateID);

	bool IsOnline(UINT64 roleid);
	void BroadCast(const CProtocol& ptc, UINT32 line = 0);


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