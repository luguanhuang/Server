#ifndef _LEAGUE_MATCH_PARAM_MGR_H_
#define _LEAGUE_MATCH_PARAM_MGR_H_

#include "util/utilsingleton.h"
#include "table/LeagueMatchParamTable.h"


class LeagueMatchParamMgr : public Singleton<LeagueMatchParamMgr>
{
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetK(UINT32 score, bool isWin);

private:
	LeagueMatchParamTable m_table;
	std::map<UINT32, LeagueMatchParamTable::RowData*> m_scoreMap;
};

#endif