#ifndef __REWARDHANDLER_H__
#define __REWARDHANDLER_H__

struct SRewardInfo;
class CRewardRecord;
class Role;
class CRewardHandler
{
public:
	CRewardHandler(UINT32 dwType);
	virtual ~CRewardHandler();
	
	///> 判断记录过期（过期:重置且重新计算奖励，没过期:跳过）
	virtual bool Update(CRewardRecord* pRecord, UINT32 dwType) { return true; }
	///> 计算玩家奖励
	virtual bool GetReward(Role* pRole, SRewardInfo& stRewardInfo);
	///> 定时更新和重置
	bool OnTimer(UINT32 dwType);

protected:
	UINT32 m_dwType;
};

///> 午餐奖励，定点更新
class CDinnerRewardHandler : public CRewardHandler
{
public:
	CDinnerRewardHandler(UINT32 dwType);
	virtual ~CDinnerRewardHandler();

	virtual bool Update(CRewardRecord* pRecord, UINT32 dwType);
	virtual bool GetReward(Role* pRole, SRewardInfo& stRewardInfo);

private:
	UINT32 m_begin;
	UINT32 m_end;
	UINT32 m_crossday;
};

///> 晚餐奖励，定点更新
class CSupperRewardHandler : public CRewardHandler
{
public:
	CSupperRewardHandler(UINT32 dwType);
	virtual ~CSupperRewardHandler();

	virtual bool Update(CRewardRecord* pRecord, UINT32 dwType);
	virtual bool GetReward(Role* pRole, SRewardInfo& stRewardInfo);

private:
	UINT32 m_begin;
	UINT32 m_end;
	UINT32 m_crossday;
};

#endif