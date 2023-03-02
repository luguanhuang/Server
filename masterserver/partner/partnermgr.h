#ifndef _PARTNER_MGR_H_
#define _PARTNER_MGR_H_

#include "util/utilsingleton.h"
#include "util/maplist.h"
#include "partner.h"
#include "timer.h"
#include "util/simpletimer.h"


class PartnerMgr : public ITimer, public Singleton<PartnerMgr>
{
public:
	PartnerMgr();
	~PartnerMgr();

	bool Init();
	void Uninit();
	void SaveAll2DB();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool LoadFromDB();

	void CheckDayPass();
	void StartMakePartner(std::vector<UINT64>& roleIDs);
	Partner* MakePartner(std::vector<UINT64>& roleIDs);
	KKSG::ErrorCode CheckMakePartner(UINT64 roleId, std::vector<UINT64>& parnterIds);
	KKSG::ErrorCode CheckMakePartner(std::vector<UINT64>& partnerIds, UINT64& errorRoleId);

	bool HasPartner(UINT64 roleID);
	bool IsPartner(UINT64 roleID1, UINT64 roleID2);

	UINT64 GetPartnerIdByRole(UINT64 roleID);
	Partner* GetPartnerByRole(UINT64 roleID);
	Partner* GetPartner(UINT64 partnerID);

	void AddFriendDegree(UINT64 roleId1, UINT64 roleId2, UINT32 value);

	void AddToUpdateList(UINT64 partnerId)
	{
		m_dirtyList.push_back(partnerId);
	}
	void AddToDeleteList(UINT64 partnerId)
	{
		m_delList.push_back(partnerId);
		m_dirtyList.remove(partnerId);
	}

	static void NotifyMakePartnerError(std::vector<UINT64>& roleIds, KKSG::ErrorCode code, UINT64 roleId);

	void AddRolePartner(UINT64 roleId, UINT64 id);
	void RemoveRolePartner(UINT64 roleId);

	void AddToLeaveTimeList(UINT64 roleId, UINT32 time);
	void RemoveFromLeaveTimeList(UINT64 roleId);
	void OnLeaveTimeOut(UINT64 roleId);

	bool OnDayPass();
	bool RefreshShop();

	UINT32 GetLastRefreshShopTime() { return m_lastPassDayTime; }

private:
	UINT64 NewID();
	void AddPartner(UINT64 id, Partner* partner);

private:
	UINT32 m_curId;
	UINT32 m_lastPassDayTime;
	std::unordered_map<UINT64, Partner*> m_partners;
	std::unordered_map<UINT64, UINT64>	m_roleId2partnerId;

	MapList<UINT64> m_dirtyList;
	MapList<UINT64> m_delList;
	SimpleTimer<UINT64, UINT32> m_leaveTimer;
};

#endif