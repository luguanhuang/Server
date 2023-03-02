#ifndef __REWARDRECORD_H__
#define __REWARDRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "gamelogic/item.h"
#include "utility/BitSet.h"

#define MAXREWARDBIT 1024

enum ERewardState
{
	eCanNotGet = 0,
	eCanGet = 1,
	eHasGet = 2,
};

enum ERewardSubType
{
	eCanCover = 1,
	eCanNotCover = 2,
	eTimeReward = 3,
};

namespace KKSG
{
	class RewardInfo;
}
struct SRewardInfo
{
	bool   isget;
	UINT32 uniqueday;					///> 领取后标记本周起内已领取

	UINT64 qwUniqueId;					///> 记录产生的唯一Id
	UINT32 dwType;						///> 奖励对应的系统类型
	UINT32 dwSubType;					///> 系统的子类型（可覆盖奖励和不可覆盖奖励）
	UINT32 dwState;						///> 领奖状态（0：不可领取，1：可领取，2：已领取）
	UINT32 dwTimeStamp;					///> 记录更新时间
	std::vector<std::string> vecParam;	///> 各个系统记录的参数
	std::vector<ItemDesc> vecItem;		///> 奖励的物品

	SRewardInfo();

	void Reset();
	bool ResetDay();
	void SetDay(const SRewardInfo& info);

	///> 验证记录是否过期（每日更新）
	bool Update(int nHour, int nMin, bool& isdayreset);

	void RewardInfoToKKSG(KKSG::RewardInfo* pInfo) const;
	void KKSGToRewardInfo(const KKSG::RewardInfo* pInfo);
};

class Role;
class CRewardRecord : public RoleSerializeInterface
{
public:
	CRewardRecord(Role* pRole);
	~CRewardRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	bool Init(UINT32 dwType, UINT32 dwSubType);

	void Push(SRewardInfo& stRewardInfo, bool bIsNotfyClient = true);	
	bool Pop(UINT64 qwUniqueId);
	
	bool Update(UINT32 dwType, int nHour, int nMin);
	
	void SendAllRewardInfo();

	SRewardInfo* GetRewardInfobyId(UINT64 qwUniqueId);

	void SetGiven(UINT64 id);
	void SetTaken(UINT64 id);

	bool IsGiven(UINT64 id) const;
	bool IsTaken(UINT64 id) const;

	bool IsTakenNextDay() { return m_nextdayreward > 0; }
	bool IsTakenOnline(UINT32 index);

	void SetNextDay();
	void SetOnline(UINT32 index);

private:
	Switch m_bIsModify;
	Role* m_pRole;

	UINT32 m_nextdayreward;
	std::set<UINT32> m_onlinereward;

	BitSet<MAXREWARDBIT> m_isgiven;
	BitSet<MAXREWARDBIT> m_istaken;
	std::map<UINT64, SRewardInfo> m_mapRewardInfo;		
};

#endif