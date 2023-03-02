#ifndef __OPENSYSTEMMGR_H__
#define __OPENSYSTEMMGR_H__

#include "OpenSystemTable.h"
#include "unit/role.h"

typedef OpenSystemTable::RowData OpenSystemConf;

class CRoleSystem;
struct ItemDesc;
namespace KSKG
{
	class RoleAllInfo;
}
class OpenSystemMgr
{
	friend class CRoleSystem;
	OpenSystemMgr();
	~OpenSystemMgr();
	DECLARE_SINGLETON(OpenSystemMgr)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	bool OnTimer(std::vector<OpenSystemConf*>& systemID);

	void OnGetGameServerOpenTime(bool force = false);
	void OnRoleFirstEnterScene(Role* pRole);

	const std::vector<int> &OpendSystemWhenCreateRole() { return m_initSystem; }
	void CheckOpenSystem(int level, int taskid, std::vector<int>& systemids);
	int GetPlayerLevelBySystemID(int systemID);
	const std::vector<ItemDesc>* GetReward(UINT32 id); 

	//idip
	bool IsClosedSystem(UINT32 systemID);
	bool IsClosedSystem(UINT32 systemID, UINT64 roleID);
	void IdipOpSystem(UINT32 systemID, bool opOpen);
	void ExceptCloseSystem(Role* role, KKSG::RoleAllInfo* info);

	void NotifyClientSystemOpen(Role* role, std::vector<UINT32>* openids, std::vector<UINT32>* closeids);
	void NotifyMsSystemOpen(Role* role, std::vector<UINT32>* openids);
	void OnHallEnterScene(Role* role);

	UINT32 GetOpenDay(OpenSystemTable::RowData* pConf);

private:
	bool _IsIdipCloseSystem(UINT32 systemID);
	void _LoadIdipOpSystem();
	void _OnSystemOpenDay(int systemID);
	void _ExceptIdipCloseSystem(Role* role, KKSG::RoleAllInfo* info);
	void _GetClosedSystem(std::vector<UINT32>& closeSys);

private:
	std::map<UINT32, std::vector<ItemDesc>> m_id2reward;
	std::vector<int> m_initSystem;

	OpenSystemTable  m_oSystemTable;

	std::vector<OpenSystemConf*> m_startedSystem;

	std::unordered_set<UINT32> m_handler;

	std::set<UINT32> m_idipSystemClose;

	int m_loadCnt;
	int m_getServerOpenTimeCnt;
};

#endif // __OPENSYSTEMMGR_H__

