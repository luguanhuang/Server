#ifndef __BATTLEFIELD_RECORD_H__
#define __BATTLEFIELD_RECORD_H__

#include "unit/roleserilizeinterface.h"
#include "pb/project.pb.h"
#include "unit/role.h"


class CBattleFieldRecord : public RoleSerializeInterface
{
public:
	CBattleFieldRecord(Role* pRole);
	~CBattleFieldRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	void ResetDropInfo();
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	void OnDayPass();
	void AddFirstRankCount();
	void AddTotalKillCount();
	UINT32 GetPointIdCount(UINT32 id);
	void AddPointIdCount(UINT32 id);
	KKSG::HellDropInfo* GetDropInfo(UINT32 nId); 
	void SetDropInfo(UINT32 nId,UINT32 nCount);
	void GivePointAward(UINT32 nPoint);

	void FillBattleReward(KKSG::BattleFieldAwardNumRes &res);
	void ClearBattleReward();
private:
	Role* m_pRole;
	Switch m_isModified;
	UINT32 m_LastUpdateWeekTime;
	UINT32 m_FirstRankCount;
	UINT32 m_TotalKillCount;
	UINT32 m_LastUpdateDayTime;
	std::map<UINT32, UINT32> m_mapPoint;
	std::map<UINT32, KKSG::HellDropInfo> m_mapDropInfo;
	std::map<UINT32,UINT32> m_mapGotAward;


};
#endif
