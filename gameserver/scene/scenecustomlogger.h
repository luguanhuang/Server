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
	UINT64 m_MatchId;//����ID///
	INT32 m_Matchtype;//��������1v1,2v2///
	INT32 m_MatchMode;//��ƽģʽ; 0:��ƽģʽ 1���ǹ�ƽģʽ///
	INT32 m_isSystem;//�Ƿ�ϵͳ����;//0:�� 1����
	//UINT32 m_ConfigId;//��������ID;

	INT32 m_isWin;//��������;
	INT32 m_Score;//��������;
	INT32 m_TotalScore;//�ܻ���;

	std::map<UINT64, TCustomPkResult> m_roleid2log;
};

#endif