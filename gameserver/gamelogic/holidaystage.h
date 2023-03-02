#ifndef __HOLIDAYSTAGE_H__
#define __HOLIDAYSTAGE_H__
#include "table/FestScene.h"
#include "timer.h"
//#include "unit/roleserilizeinterface.h"

class Role;

class CHolidayStage 
{
	CHolidayStage();
	~CHolidayStage();
	DECLARE_SINGLETON(CHolidayStage);

public:
	bool Init();
	void Uninit();
	
public:
	bool IsHolidaySecneId(UINT32 sceneId);
	UINT32 GetHolidaySceneId();
	UINT32 GetHolidayId();
	UINT32 GetLimitTimes();
	UINT32 GetLostTimes(UINT64 roleId, UINT32 sceneId);
	UINT32 GetHaveTime();
	void CheckOnline(Role *poRole);
	bool CheckFile();
	bool LoadFile();
	
	bool OnStart(UINT32 active_id);
	bool OnEnd(UINT32 active_id);
private:
	void _ReCheck();
	bool _IsHolidayOpen(UINT32 id); 
	void _CreateEndTime();
private:
	std::map<UINT32,UINT32> start_handler_;
	std::map<UINT32,UINT32> end_handler_;
	std::map<UINT32, UINT32> m_sealMap; // 活动等级封印
	FestScene m_festSceneTable;         // 节日活动表
};

#endif
