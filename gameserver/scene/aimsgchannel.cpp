#include "pch.h"
#include "aimsgchannel.h"
#include "scenefinder.h"
#include "scene/scene.h"
#include "unit/unit.h"
#include "gamelogic/XLevelWave.h"
#include "entity/XActionDefine.h"
#include "entity/XObject.h"
#include "component/XAIComponent.h"

AISceneMsgChannel::AISceneMsgChannel()
{
	m_Scene = NULL;
}

AISceneMsgChannel::~AISceneMsgChannel()
{
	m_Scene = NULL;
};

bool AISceneMsgChannel::SendMsgTo(AIMsgTarget msgTargetType, AIMsgType msgType, XAIEventArgs& args)
{
	std::vector<Unit*> msgTargets;
	FindEnemyCondition enemy;

	switch (msgTargetType)
	{
	case Ally:
	case Role:
		break;
	case Boss:
		for (auto it=m_BossId.begin(); it != m_BossId.end(); it++)
		{
			Unit* boss = Unit::FindUnit(*it);
			if (boss)
				msgTargets.push_back(boss);
		}
		break;
	case Monster:
	case Enemy:
	case All:
		m_Scene->GetUnitByCondition(enemy, msgTargets);
		break;
	case LevelMgr:
		if (m_Scene->GetSpawner() && msgType == ExString)
			m_Scene->GetSpawner()->m_oLevelState.SetExternalString(args.EventArg);
		return true;
	case GlobalAI:
		if (m_Scene->GetGlobalAI())
			msgTargets.push_back(m_Scene->GetGlobalAI());
		break;
	default:
		break;
	};

	for (UINT32 i=0; i<msgTargets.size(); i++)
	{
		if (args.MsgTargetTemplateId != 0 && (int)msgTargets[i]->GetTemplateID() != args.MsgTargetTemplateId && msgTargetType != GlobalAI)
			continue;

		SendMsgTo(msgTargets[i], msgType, args);
	}

	return true;
}

bool AISceneMsgChannel::SendMsgTo(Unit* pUnit, AIMsgType msgType, XAIEventArgs& args)
{
	if (pUnit == NULL)
		return false;
	XAIEventArgs* aievent = new XAIEventArgs();
	aievent->EventType = msgType;
	aievent->Templateid = args.Templateid;
	aievent->Pos = args.Pos;
	aievent->EventArg = args.EventArg;
	aievent->WaveId = args.WaveId;
	aievent->UnitId = args.UnitId;
	aievent->SkillId = args.SkillId;
	aievent->EnemyTemplateId = args.EnemyTemplateId;
	aievent->FloatArg = args.FloatArg;
	aievent->SenderUID = args.SenderUID;

	HTIMER token = pUnit->GetXObject()->DelayAction(aievent, args.DelayTime>0 ? args.DelayTime : 0.05f);
	pUnit->GetXObject()->AIComponent()->AddTimerToken(token);

	return true;
}

void AISceneMsgChannel::AddSceneBoss(Unit* boss)
{
	if (m_BossId.find(boss->GetID()) == m_BossId.end())
		m_BossId.insert(boss->GetID());
}

void AISceneMsgChannel::RemoveSceneBoss(Unit* boss)
{
	if (m_BossId.find(boss->GetID()) != m_BossId.end())
		m_BossId.erase(boss->GetID());
}




