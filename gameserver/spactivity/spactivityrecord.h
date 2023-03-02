#ifndef __SPACTIVITYRECORD_H__
#define __SPACTIVITYRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "spactivityconfig.h"
#include "ExpeditionTable.h"


class Role;
class SpActivityDataBase;

class SpActivityRecord : public RoleSerializeInterface
{
public:
	SpActivityRecord(Role* pRole);
	~SpActivityRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	inline void SetModify() { m_isModify.Set(); }

	void UpdatePerMinute();
	void OnFirstEnterScene();

	SpActivityDataBase* GetSpActData(UINT32 actid);

	SpActivityDataBase* AddAndGetSpActData(UINT32 actid, UINT32 startTime, UINT32 stage);
	
	void AddOrUpdateSpActivityTask(UINT32 actid, UINT32 startTime, UINT32 stage, const std::vector<SpActivityTaskConf*>* allTask);
	void DeleteOldSpAct(UINT32 actid);

	void OnIncreaseProgress(const std::vector<SpActivityTaskConf*>& tasks, UINT32 count = 1);
	void OnDecreaseProgress(const std::vector<SpActivityTaskConf*>& tasks, UINT32 count = 1);

	UINT32 GetSpActStartTime(UINT32 actid) const;
	UINT32 GetSpActStage(UINT32 actid) const;

	void AddSpActStage(UINT32 actid);
	void ResetSpActTask(UINT32 actid);

	KKSG::ErrorCode GetReward(UINT32 actid, UINT32 taskid, const std::vector<Sequence<UINT32, 2>>& reward, int subreason);

	const std::map<UINT32, SpActivityDataBase*>& GetAllSpActData() const { return m_actid2Data; }

	inline UINT32 GetLastBackFlowStartTime() const { return m_lastBackFlowStartTime; }
	void SetLastBackFlowStartTime(UINT32 backFlowTime) { m_lastBackFlowStartTime = backFlowTime; m_isModify.Set(); }

	inline UINT32 GetLastArgentaStartTime() const { return m_lastArgentStartTime; }
	void SetLastArgentaStartTime(UINT32 argentaTime) { m_lastArgentStartTime = argentaTime; m_isModify.Set(); }	

	///> 塔姐的福利
	void UpdateArgentaPreData();
	void OnAddActivityPoint(UINT32 point);
	void OnFinishNest();
	bool CheckArgentaOpen();
	void GMPrintArgentaPreData(std::string& data);

	///> 回流活动触发
	void UpdateBackFlowPreData(bool isLogin = false);
	bool CheckBackFlowOpen();
	void GMPrintBackFlowPreData(std::string& data);

	///> 日常活动
	void CheckSendTickets();
	bool CheckBioHellOpen(ExpeditionTable::RowData* pRow);
	bool CheckIsFirstCompleteScene(UINT32 actid, UINT32 scenetype, UINT32 sceneid);
	void UpdateFirstCompleteScene(UINT32 actid, UINT32 scenetype, UINT32 sceneid);

	///> GM
	void GMClear();
	void ResetAncientTimesAward();
	void GMCompleteAll();

private:
	void _OnSpActivityOpen(SpActivityDataBase* base);
	void _OnSpActivityClose(SpActivityDataBase* base);
	void _LotteryDraw(UINT32 m_actid);//转盘活动专用

private:
	Role* m_pRole;
	Switch m_isModify;

	std::map<UINT32, SpActivityDataBase*> m_actid2Data;

	UINT32 m_lastBackFlowStartTime;

	UINT32 m_lastArgentStartTime;

	KKSG::ArgentaPreData m_argentaPreData;
	KKSG::BackFlowPreData m_backflowPreData;
};

#endif