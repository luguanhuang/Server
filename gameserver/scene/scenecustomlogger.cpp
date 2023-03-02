#include "pch.h"
#include "scenecustomlogger.h"

CustomBattleLogger::CustomBattleLogger()
{

}

CustomBattleLogger::~CustomBattleLogger()
{

}

void CustomBattleLogger::Init(UINT64 uid, UINT32 type, bool isfair, bool issystem)
{
	m_MatchId = uid;//����ID///
	m_Matchtype = type;//��������1v1,2v2///
	m_MatchMode = isfair ? 0 : 1;//��ƽģʽ; 0:��ƽģʽ 1���ǹ�ƽģʽ///
	m_isSystem = issystem ? 1 : 0;//�Ƿ�ϵͳ����;//0:�� 1����
}

void CustomBattleLogger::PushRole(Role* role)
{
	if (NULL != role)
	{
		m_roleid2log[role->GetID()].SetRoleData(role);
	}
}

void CustomBattleLogger::PushScore(UINT64 roleid, bool iswin, int point, int allpoint)
{
	m_roleid2log[roleid].SetMatchData(m_MatchId, m_Matchtype, m_MatchMode, m_isSystem);
	m_roleid2log[roleid].SetMatchResult(roleid, iswin ? 1 : 0, point, allpoint);
}

void CustomBattleLogger::End()
{
	for (auto i = m_roleid2log.begin(); i != m_roleid2log.end(); ++i)
	{
		i->second.BuildAndSend();
	}
}

