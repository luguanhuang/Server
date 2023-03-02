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
	UINT32 uniqueday;					///> ��ȡ���Ǳ�����������ȡ

	UINT64 qwUniqueId;					///> ��¼������ΨһId
	UINT32 dwType;						///> ������Ӧ��ϵͳ����
	UINT32 dwSubType;					///> ϵͳ�������ͣ��ɸ��ǽ����Ͳ��ɸ��ǽ�����
	UINT32 dwState;						///> �콱״̬��0��������ȡ��1������ȡ��2������ȡ��
	UINT32 dwTimeStamp;					///> ��¼����ʱ��
	std::vector<std::string> vecParam;	///> ����ϵͳ��¼�Ĳ���
	std::vector<ItemDesc> vecItem;		///> ��������Ʒ

	SRewardInfo();

	void Reset();
	bool ResetDay();
	void SetDay(const SRewardInfo& info);

	///> ��֤��¼�Ƿ���ڣ�ÿ�ո��£�
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