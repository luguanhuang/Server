#ifndef __SURVIVEMGR_H__
#define __SURVIVEMGR_H__

#include "util/utilsingleton.h"
#include "timer.h"
#include "table/ChickenDinnerRankReward.h"

class SurviveRankList;

class CSurviveMgr : public Singleton<CSurviveMgr>, public ITimer
{
public:
	CSurviveMgr();
	~CSurviveMgr();

	bool Init();
	void Uninit();

	bool LoadFile();
	bool CheckConfig();
	void ClearConfig();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool ReadData();
	void WriteData();

	void WeekZero();
	void GiveReward(UINT64 roleID, UINT32 rank);

	SurviveRankList* GetRankList();	

	void InfoPrint();

private:

	ChickenDinnerRankReward::RowData* GetConf(UINT32 rank);

private:
	HTIMER m_handler;
	KKSG::SurviveGlobal m_data;
	ChickenDinnerRankReward m_rrTable;	
};

#endif