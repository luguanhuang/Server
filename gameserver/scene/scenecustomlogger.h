#ifndef __SCENECUSTOMLOGGER_H__
#define __SCENECUSTOMLOGGER_H__

#include "utility/tlogger.h"

class CustomBattleLogger
{
public:
	CustomBattleLogger();
	~CustomBattleLogger();

	void Init(UINT64 uid, UINT32 type, bool isfair, bool issystem);

	void PushRole(Role* role);
	void PushScore(UINT64 roleid, bool iswin, int point, int allpoint);
	
	void End();

private:
	UINT64 m_MatchId;//比赛ID///
	INT32 m_Matchtype;//比赛类型1v1,2v2///
	INT32 m_MatchMode;//公平模式; 0:公平模式 1：非公平模式///
	INT32 m_isSystem;//是否系统比赛;//0:否 1：是
	//UINT32 m_ConfigId;//比赛配置ID;

	INT32 m_isWin;//本场积分;
	INT32 m_Score;//本场积分;
	INT32 m_TotalScore;//总积分;

	std::map<UINT64, TCustomPkResult> m_roleid2log;
};

#endif