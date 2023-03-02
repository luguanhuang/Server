#ifndef __SPACTIVITYSMALL_H__
#define __SPACTIVITYSMALL_H__

#include "spactivitybase.h"
#include "table/ArgentaDaily.h"
#include "table/ArgentaTask.h"
#include "spactivitydatasmall.h"
#include "table/OpenServerActivity.h"
#include "table/WeekEndNestActivity.h"
#include "table/BackFlowShop.h"

class SpActivityOpenServer : public SpActivityBase
{
public:
	SpActivityOpenServer(UINT32 actid);
	virtual ~SpActivityOpenServer();

	virtual bool Init();

	bool CheckFile();
	virtual bool LoadFile();
	void ClearFile();

	virtual void OnStart();
	virtual void UpdateSpActivityData(Role* pRole, bool updateTask = false);

private:
	OpenServerActivity m_oTable;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActivityPartnerDay : public SpActivityBase
{
public:
	SpActivityPartnerDay(UINT32 actid);
	virtual ~SpActivityPartnerDay();

	virtual void OnNextStage();

	virtual KKSG::ErrorCode GetReward(Role* pRole, UINT32 taskid);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef BackflowShop::RowData BackflowShopConf;

class SpActivityBackFlow : public SpActivityBase
{
public:
	SpActivityBackFlow(UINT32 actid);
	virtual ~SpActivityBackFlow();

	virtual bool Init();

	bool CheckFile();
	virtual bool LoadFile(bool isReload = true);
	void ClearFile();

	virtual void OnStart();
	virtual void OnEnd();

	virtual bool CheckOpenCondition(Role* pRole);
	virtual void UpdateSpActivityData(Role* pRole, bool updateTask = false);

	virtual KKSG::ErrorCode GetReward(Role* pRole, UINT32 taskid);

	void FillBackFlowActivityTask(UINT32 worldLevel, std::vector<SpActivityTaskConf*>& tasks);

	const BackflowShopConf* GetBackflowShopConf(UINT32 goodid);

	void GetNewShopGoodsList(Role* pRole, UINT32 lostDay, std::vector<UINT32>& newGoods);

private:
	BackflowShop m_oBackFlowShop;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActivityArgenta : public SpActivityBase
{
public:
	SpActivityArgenta(UINT32 actid);
	virtual ~SpActivityArgenta();

	virtual bool Init();

	bool CheckFile();
	virtual bool LoadFile();
	void ClearFile();

	virtual void OnStart();
	virtual void OnEnd();

	virtual bool CheckOpenCondition(Role* pRole);
	virtual void UpdateSpActivityData(Role* pRole, bool updateTask = false);

	virtual KKSG::ErrorCode GetReward(Role* pRole, UINT32 taskid);

public:
	ArgentaDaily m_oDailyTable; 
	ArgentaTask m_oTaskTable;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActivityAncient : public SpActivityBase
{
public:
	SpActivityAncient(UINT32 actid);
	virtual ~SpActivityAncient();

	virtual bool Init();

	bool CheckFile();
	virtual bool LoadFile();
	void ClearFile();

	void OnActivityOpen(Role* pRole);

	UINT32 GetAcientTimesTableSize();
	std::vector<Sequence<uint, 2>> GetAcientTimesAward(Role* pRole, UINT32 nPos);

private:
	AncientTimesTable m_oAncientTimes;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActivityTheme : public SpActivityBase
{
public:
	SpActivityTheme(UINT32 actid);
	virtual ~SpActivityTheme();

	virtual void OnNextStage();
	virtual void UpdateSpActivityData(Role* pRole, bool updateTask = false);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActivityWeekEndNest : public SpActivityBase
{
public:
	SpActivityWeekEndNest(UINT32 actid);
	virtual ~SpActivityWeekEndNest();
	
	virtual void OnStart();

	virtual bool Init();

	bool CheckFile();
	virtual bool LoadFile();
	void ClearFile();

	virtual KKSG::ErrorCode GetReward(Role* pRole, UINT32 taskid);

	virtual void UpdateSpActivityData(Role* pRole, bool updateTask = false);

	void GetAllTask(std::vector<SpActivityTaskConf*>& allTask);

private:
	WeekEndNestActivity m_oWeekEndNest;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class SpActivityLuckyTurntable : public SpActivityBase
{
public:
	SpActivityLuckyTurntable(UINT32 actid);
	virtual ~SpActivityLuckyTurntable();

	void GetItemRecord(std::vector<KKSG::ItemRecord>& all);
	virtual void UpdateSpActivityData(Role* pRole, bool updateTask = false);
	void FillConsume(KKSG::GetLuckyActivityInfoRes& res, UINT32 state);
	KKSG::ErrorCode GetPriceAndConsume(UINT32 state, UINT32 &price, UINT32 &consumeitemid);
	void LotterDraw(KKSG::LotteryDrawRes& res, Role *pRole);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif