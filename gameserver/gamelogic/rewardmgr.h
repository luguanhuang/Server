#ifndef __REWARDMGR_H__
#define __REWARDMGR_H__

#include "rewardhandler.h"
#include "rewardrecord.h"
#include "table/SystemRewardTable.h"
#include "table/TimeRewardTable.h"

/****************
* 1. update reward(reward handler)
* 2. once reward(once reward mgr)
* 3. timer reward(on timer)
*****************/

class COnceRewardMgr
{
public:
	COnceRewardMgr();
	~COnceRewardMgr();

	void ClearReward(UINT64 qwRoleId);

	void PushReward(UINT64 qwRoleId, const SRewardInfo& stRewardInfo, bool bIsSaveToDb = false);
	bool PopReward(UINT64 qwRoleId, UINT64 qwUniqueId);

	void AddToClient(UINT64 qwRoleId, const SRewardInfo& stRewardInfo);
	void RemoveToClient(Role* pRole, UINT64 qwUniqueId);
	void SendAllToClient(UINT64 qwRoleId);

private:
	std::unordered_map<UINT64, std::unordered_map<UINT64, SRewardInfo>> m_mapRole2Rewards;
};

class Role;
class CRewardRecord;
struct ItemDesc;
class CRewardMgr 
{
	CRewardMgr();
	~CRewardMgr();
	DECLARE_SINGLETON(CRewardMgr);
public:
	bool Init();
	void Uninit();

	///> 角色上线，系统奖励初始化
	void OnRoleLogin(Role* pRole);

	void Dispatch(UINT32 type);

	///> 一次性奖励
	//void ReceiveFromDb(UINT64 qwRoleId, const SRewardInfo& stRewardInfo);
	//bool RemoveFromDb(UINT64 qwRoleId, UINT64 qwUniqueId);
	//void ClearOnceReward(UINT64 qwRoleId);
	//void SendOnceReward(UINT64 qwRoleId);

	//void PushOnceReward(UINT64 qwRoleId, UINT32 dwType, const std::vector<std::string>& vecParam
	//	, const std::vector<ItemDesc>& vecItem);

	///> 奖励物品内部获取
	void PushReward(UINT64 qwRoleId, UINT32 dwType, bool bIsNotifyClient = true);
	void PushReward(Role* pRole, UINT32 dwType, bool bIsNotifyClient = true);

	///> 通过配置获取默认奖励
	std::vector<ItemDesc> GetDefaultReward(UINT32 dwType);

private:
	CRewardHandler* GetHandler(UINT32 dwType);
	UINT32 GetSubType(UINT32 dwType);

private:
	SystemRewardTable m_oTable;
	
	COnceRewardMgr m_oOnceRewardMgr;
	CRewardHandler m_oDefaultHanlder;
	std::map<UINT32, CRewardHandler*> m_mapRewardHandler;
};

#endif