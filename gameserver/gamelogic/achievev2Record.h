#ifndef __ACHIEVEV2_RECORD_H__
#define __ACHIEVEV2_RECORD_H__

#include "unit/roleserilizeinterface.h"
#include <map>
#include "unit/role.h"

struct STC_ACHIEVE_V2_INFO
{
	UINT32	dwAchieveID;
	UINT32  dwReachTimestamp;
	UINT32  dwRewardStatus;
};

struct STC_ACHIEVE_POINT_REWARD_INFO
{
	UINT32 dwRewardID;
	UINT32 dwRewardStatus;
};

class AchieveV2Record : public RoleSerializeInterface
{
public:
	AchieveV2Record(Role* role);
	virtual ~AchieveV2Record();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	virtual void FirstInit(UINT32 roleNum);
	void Init();

	UINT32 GetAchivementState(int aid);
	void SetAchivementState(int aid, int state);



public:
	Role* m_pRole;

	Switch m_ismodify;

	std::map<UINT32, UINT32> m_oldachievement;	///> 老的成就数据

	std::map<UINT32, STC_ACHIEVE_V2_INFO> m_mapAchieveInfo;	
	std::map<UINT32, STC_ACHIEVE_POINT_REWARD_INFO> m_mapAchievePointRewardInfo;
};

#endif // __LOTTERYRECORD_H__