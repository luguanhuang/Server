#ifndef _PUSH_MGR_H_
#define _PUSH_MGR_H_

#include <unordered_map>
#include <map>
#include "singleton.h"
#include "WebFetchThread.h"
#include "table/PushMessageTable.h"
#include "pushhandler.h"
#include "xingepushmgr.h"
#include "timer.h"

namespace KKSG 
{
	class RolePushInfo;
}
class CRoleSummary;

typedef PushMessageTable::RowData PushMessageTableData;

class PushMgr : public ITimer
{
	PushMgr();
	~PushMgr();
	DECLARE_SINGLETON(PushMgr)

public:
	bool Init();
	void Uninit();
	void Update();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	void RolePushUpdate();
	void HandlerUpdate();

	// 等级封印解锁
	void TriggerByLevelSeal();

	void PushToRole(UINT64 roleId, PushMessageTableData* data);
	void PushToRole(UINT64 roleId, const std::string& title, const std::string& content, UINT32 type = 0);
	void PushToRole(const std::string& account, const std::string& title, const std::string& content, UINT32 type = 0);

	void PushToAll(PushMessageTableData* data);
	void PushToAll(const std::string& title, const std::string& content, UINT32 type = 0);

	void OnRoleLogin(UINT64 roleId);
	void OnRoleLogout(UINT64 roleId);

	void UpdateRolePushItem(CRoleSummary* role, const KKSG::RolePushInfo& info);
	void AddRolePushItem(UINT64 roleId, UINT32 type, UINT32 subType, UINT32 time);
	void RemoveRoleAllPushItem(UINT64 roleId);

	const std::vector<PushMessageTableData*>& GetAllTableData();
	void GetTableData(UINT32 type, std::vector<PushMessageTableData*>& data);
	void GetTableData(UINT32 type, UINT32 subType, std::vector<PushMessageTableData*>& data);

	bool PushFilter(UINT32 type, CRoleSummary* pRoleSummary);

private:
	void DoPushTask(CRoleSummary* role, const std::string& title, const std::string& content);
	void RemoveRoleTime(UINT64 roleId, UINT32 time);

private:
	struct RolePushItem
	{
		UINT64 roleId;
		UINT32 type;
		UINT64 subType;
		UINT32 time;
	};

private:
	HTIMER m_timerId;
	PushMessageTable m_config;
	std::map<UINT32, std::vector<PushMessageTableData*> > m_tableData;
	std::map<UINT32, PushHandler*> m_handlers;

	std::multimap<UINT32, RolePushItem*> m_time2PushItem;	// key:时间戳
	std::unordered_multimap<UINT64, UINT32> m_roleId2time;  // key:roleId value:时间戳

	XinGePushMgr m_xinge;
};

#endif