#ifndef __ROLEMANAGER_H__
#define __ROLEMANAGER_H__

#include "multiindex_map.h"
#include "role.h"
#include "levelseal/ptcg2c_levelsealntf.h"

namespace KKSG
{
	class RoleAllInfo;
};

class IRoleTravelCallback
{
public:
	virtual void OnIteratorRole(Role *pRole, void *parg) = 0;
};

typedef UINT32 (*FilterRole)(Role*, Role*);

class RoleManager
{
	friend class CRolePartCallBack;
private:
	RoleManager();
	~RoleManager();
	DECLARE_SINGLETON(RoleManager)
public:
	bool Init();
	void Uninit();

	void AddRole(Role* role);
	void RemoveRole(Role* role);
	void ConvertSession(Role* pRole, SessionType oldSession, SessionType newSession);
	Role *CreateRole(const KKSG::RoleAllInfo *pInfo, UINT64 sessionID, int index, const KKSG::ClientInfo* poCliInfo, const EnterSceneInfo& info, UINT32 routerConnId);
	void RemoveBySession(SessionType sessionID);
	UINT32 GetRoleCount() { return m_RoleDatas.size(); }

	Role *FindBySession(SessionType sessionID);
	Role *FindByRoleID(UINT64 roleID);
	Role *FindByName(const std::string& name);
	Role *FindByNickID(UINT64 NickID);
	Role *FindByRoleIDOrNickID(UINT64 ID);

	void Update(float delta);
	void Foreach(IRoleTravelCallback *cb, void *parg);

	bool UpdateOnDayPass();

	void BroadCast(const CProtocol& ptc);

	void LevelSealBroadCast(INT32 iType);
	void OnRouterClose(UINT32 line, UINT32 connId);
	
	void GetFilterRole(Role* me, FilterRole filter, std::vector<UINT64>& roles, std::vector<UINT64>& exroles);

	struct SessionIndex
	{
		typedef SessionType key;
		key Key(const Role *r) { return r->GetSessionID(); }
	};

	struct RoleIDIndex
	{
		typedef UINT64 key;
		key Key(const Role *r) { return r->GetID(); }
	};

	std::list<Role*>::iterator Begin() { return m_RoleDatas.begin();}
	std::list<Role*>::iterator End() { return m_RoleDatas.end();}

private:
	void _DumpRoles();
	mutliindex_map_ptr<Role, SessionIndex, RoleIDIndex> m_RoleDatas;
	std::unordered_map<std::string, UINT64> m_name2roleid;
	std::unordered_map<UINT64, UINT64> m_nick2roleid;

	UINT32 m_dwDayPassTimeSpecific;
	
};

#endif // __ROLEMANAGER_H__