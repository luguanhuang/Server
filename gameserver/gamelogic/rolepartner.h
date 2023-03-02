#ifndef _ROLE_PARTNER_H_
#define _ROLE_PARTNER_H_

#include "unit/roleserilizeinterface.h"

class Role;
class RolePartner : public RoleSerializeInterface
{
public:
	RolePartner(Role *pRole);
	~RolePartner();
	
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	UINT64 GetPartnerID() { return m_partnerID; }
	void SetPartnerID(UINT64 partnerID);
	UINT32 GetLastLeavePartnerTime() { return m_lastLeavePartnerTime; }
	void SetLastLeavePartnerTime(UINT32 lastTime);

	bool IsMakePartnerCDOk();
	bool IsChestTaked(UINT32 index);
	void SetChestTaked(UINT32 index);
	UINT32 GetChestTakeInfo() { return m_takedChest; }
	bool HasCheskCanTake(UINT32 level, UINT32 liveness);
	UINT32 GetOpenShopTime() { return m_openShopTime; }
	void SetApplyLeaveTime(UINT32 t);

	void UpdateLeaveState();
	void CheckChest(UINT32 partLevel, UINT32 liveness);

	KKSG::ErrorCode CheckCanMakePartner();

	void OnDayPass();
	void OnLogin();

private:
	Switch m_dirty;
	Role *m_role;
	UINT64 m_partnerID;
	UINT32 m_lastLeavePartnerTime;
	UINT32 m_takedChest;
	UINT32 m_openShopTime;
	UINT32 m_applyLeaveTime;
	UINT32 m_lastUpdateTime;
	bool m_hasChest;
	UINT32 m_compensateTime;	// 发送补偿的时间点
};

#endif
