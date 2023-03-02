#include "pch.h"
#include <typeinfo>
#include "ainodeaction.h"
#include "unit/unit.h"
#include "entity/XObject.h"
#include "aibehaviortree.h"
#include "component/XAIComponent.h"
#include "component/XNavigationComponent.h"
#include "entity/XEntity.h"
#include "scene/scene.h"
#include "scene/scenefinder.h"
#include "unit/enemymanager.h"
#include "scene/scenemanager.h"
#include "gamelogic/XLevelSpawnMgr.h"
#include "ainodefactory.h"
#include "scene/scenedoodad.h"
#include "unit/role.h"
#include "scene/grid.h"
#include "aionlinedebugmgr.h"
#include "scene/securitystatistics.h"
#include "scene/securityaistatistics.h"
#include "scene/sceneweekend4v4horseracing.h"
#include "aiscenehandler.h"
#include "scene/scenemobabattle.h"
#include "battle/ptcg2c_entitytargetchangentf.h"

ReceiveAIEvent::ReceiveAIEvent(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("MsgType");
	if (attr) attr->QueryIntValue(&mAIArgMsgType);

	attr = elem->FindAttribute("Deprecate");
	if (attr) attr->QueryBoolValue(&mDeprecate);

	attr = elem->FindAttribute("Shared_MsgStrName");
	if (attr) 
	{
		mAIArgMsgStr = attr->Value();
		if (mAIArgMsgStr.length() > 0)
			mStrIndex = AINodeFactory::CreateIndexByName(mAIArgMsgStr);
		else
			mStrIndex = 0;
	}

	attr = elem->FindAttribute("Shared_TypeIdName");
	if (attr) 
	{
		mAIArgTypeIdName = attr->Value();
		if (mAIArgTypeIdName.length() > 0)
			mTypeIdIndex = AINodeFactory::CreateIndexByName(mAIArgTypeIdName);
		else
			mTypeIdIndex = 0;
	}

	attr = elem->FindAttribute("Shared_TypeIdmValue");
	if (attr) attr->QueryIntValue(&mAIArgTypeId);

	attr = elem->FindAttribute("Shared_PosName");
	mPosIndex = -1;
	if (attr)
	{
		mAIArgPos = attr->Value();
		if (mAIArgPos.length() > 0)
			mPosIndex = AINodeFactory::CreateIndexByName(mAIArgPos);
	}

	attr = elem->FindAttribute("Shared_FloatArgmValue");
	if (attr) attr->QueryFloatValue(&mAIArgFloat);

	attr = elem->FindAttribute("Shared_FloatArgName");
	mFloatArgIndex = -1;

	if (attr)
	{
		mAIArgFloatArg = attr->Value();
		if (mAIArgFloatArg.length() > 0)
			mFloatArgIndex = AINodeFactory::CreateIndexByName(mAIArgFloatArg);
	}

	attr = elem->FindAttribute("Shared_SenderUIDName");
	mSenderUIDIndex = -1;
	if (attr)
	{
		string name = attr->Value();
		if (name.length() > 0)
			mSenderUIDIndex = AINodeFactory::CreateIndexByName(name);
	}

	mWaveIdIndex = AINodeFactory::CreateIndexByName("WaveId");
	mUnitIdIndex = AINodeFactory::CreateIndexByName("UnitId");
	mSkillIndex = AINodeFactory::CreateIndexByName("eventskillid");
}

bool ReceiveAIEvent::Update(Unit* unit)
{
	if (!unit->AIComponent())
		AIRETURN("ReceiveAIEvent", false);

	XAIEventArgs& args = unit->AIComponent()->AIEvent();

	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (args.EventArg == "")
		AI_DEBUG_RETURN_WITHOUT_DEFINE("ReceiveAIEvent", false);

	//if (unit->IsEnemy())
	//{
	//	LogInfo("Received ai event:%s, templateid: %d", args.EventArg.c_str(), unit->GetTemplateID());
	//}
	
	AIBehaviorTree* behaviorTree = unit->BehaviorTree();

	behaviorTree->Data().SetStringByIndex(mStrIndex, args.EventArg);
	behaviorTree->Data().SetIntByIndex(mTypeIdIndex, args.Templateid);
	behaviorTree->Data().SetVector3ByIndex(mPosIndex, args.Pos);
	behaviorTree->Data().SetIntByIndex(mSkillIndex, args.SkillId);
	behaviorTree->Data().SetFloatByIndex(mFloatArgIndex, args.FloatArg);
	behaviorTree->Data().SetStringByIndex(mSenderUIDIndex, ToString(args.SenderUID));

	if (args.UnitId != 0)
	{
		behaviorTree->Data().SetIntByIndex(mWaveIdIndex, args.WaveId);
		behaviorTree->Data().SetStringByIndex(mUnitIdIndex, ToString(args.UnitId));
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "MsgStr " << args.EventArg << "; TypeID " << args.Templateid << "; SenderUID " << args.SenderUID << "; ");

	if (mDeprecate)
		args.EventArg = "";

	AI_DEBUG_RETURN_WITHOUT_DEFINE("ReceiveAIEvent", true);
}

SendAIEvent::SendAIEvent(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("MsgTo");
	if (attr) attr->QueryIntValue(&mAIArgMsgTo);

	attr = elem->FindAttribute("MsgType");
	if (attr) attr->QueryIntValue(&mAIArgMsgType);

	attr = elem->FindAttribute("EntityTypeId");
	if (attr) attr->QueryIntValue(&mAIArgEntityTypeId);

	attr = elem->FindAttribute("MsgStr");
	if (attr) mAIArgMsgStr = attr->Value();

	attr = elem->FindAttribute("DelayTime");
	if (attr) attr->QueryFloatValue(&mAIArgDelayTime);

	attr = elem->FindAttribute("Shared_PosName");
	mPosIndex = -1;

	if (attr) 
	{
		string posName(attr->Value());
		if (posName.length() > 0)
		{
			mPosIndex = AINodeFactory::CreateIndexByName(posName);
			mPos = Vector3::zero;
		}
		else
		{
			if (elem->FindAttribute("Shared_PosmValue") != NULL)
				SharedData::ParseVector3FromString(elem->FindAttribute("Shared_PosmValue")->Value(), mPos);
		}
	}

	mWaveIdIndex = AINodeFactory::CreateIndexByName("WaveId");
	mUnitIdIndex = AINodeFactory::CreateIndexByName("UnitId");

}

bool SendAIEvent::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("ReceiveAIEvent", false);

	AIBehaviorTree* behaviorTree = unit->BehaviorTree();
	int WaveId = behaviorTree->Data().GetIntByIndex(mWaveIdIndex, 0);
	const string& oldUnit = behaviorTree->Data().GetStringByIndex(mUnitIdIndex);
	UINT64 oldUnitId = convert<UINT64>(oldUnit);
	
	if (mPosIndex != -1)
		mPos = behaviorTree->Data().GetVector3ByIndex(mPosIndex);

	XAIEventArgs args;
	args.MsgTargetTemplateId = mAIArgEntityTypeId;
	args.EventArg = mAIArgMsgStr;
	args.Templateid = mAIArgEntityTypeId;
	args.Pos = mPos;
	args.DelayTime = mAIArgDelayTime;
	args.UnitId = oldUnitId;
	args.WaveId = WaveId;
	args.FloatArg = mFloatArg;

	//LogInfo("Will send ai event, arg:%s, templateid:%d", mAIArgMsgStr.c_str(), mAIArgEntityTypeId);

	AI_DEBUG_RETURN("SendAIEvent", unit->GetCurrScene()->MsgChannel().SendMsgTo((AISceneMsgChannel::AIMsgTarget)mAIArgMsgTo, (AISceneMsgChannel::AIMsgType)mAIArgMsgType, args)); 
}

CallMonster::CallMonster(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_DistName");
	if (attr) DistName = attr->Value();

	attr = elem->FindAttribute("Shared_DistmValue");
	if (attr) attr->QueryFloatValue(&DistValue);

	attr = elem->FindAttribute("Shared_AngleName");
	mAngleIndex = -1;

	if (attr)
	{
		ArgAngleName = attr->Value();

		if (ArgAngleName.length() > 0)
			mAngleIndex = AINodeFactory::CreateIndexByName(ArgAngleName);
	}

	attr = elem->FindAttribute("Shared_AnglemValue");
	if (attr) attr->QueryFloatValue(&AngleValue);

	attr = elem->FindAttribute("MonsterId");
	if (attr) attr->QueryIntValue(&MonsterId);

	attr = elem->FindAttribute("Shared_MonsterId2Name");
	MonsterIDIndex = -1;

	if (attr)
	{
		MonsterIDName = attr->Value();

		if (MonsterIDName.length() > 0)
			MonsterIDIndex = AINodeFactory::CreateIndexByName(MonsterIDName);
	}

	attr = elem->FindAttribute("CopyMonsterId");
	if (attr) attr->QueryIntValue(&CopyMonsterId);

	attr = elem->FindAttribute("MaxMonsterNum");
	if (attr) attr->QueryIntValue(&MaxMonsterNum);

	attr = elem->FindAttribute("LifeTime");
	if (attr) attr->QueryFloatValue(&MonsterLifeTime);

	attr = elem->FindAttribute("DelayTime");
	if (attr) attr->QueryFloatValue(&MonsterDelayTime);

	attr = elem->FindAttribute("ForcePlace");
	if (attr) attr->QueryBoolValue(&ForcePlace);

	attr = elem->FindAttribute("Shared_PosName");
	mEventPosIndex = -1;
	if (attr)
	{
		EventPosName = attr->Value();
		if (EventPosName.length() > 0)
			mEventPosIndex = AINodeFactory::CreateIndexByName(EventPosName);
	}
	attr = elem->FindAttribute("Shared_PosmValue");
	if (attr)
	{
		SharedData::ParseVector3FromString(attr->Value(), EventPos);
	}
	else
	{
		EventPos = Vector3::zero;
	}

	attr = elem->FindAttribute("BornType");
	if (attr) attr->QueryIntValue(&BornPosType);

	attr = elem->FindAttribute("Pos1");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), pos[0]);

	attr = elem->FindAttribute("Pos2");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), pos[1]);

	attr = elem->FindAttribute("Pos3");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), pos[2]);

	attr = elem->FindAttribute("Pos4");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), pos[3]);

	attr = elem->FindAttribute("FinalPos");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), FinalPos);
	else FinalPos.set(0, 0, 0);

	attr = elem->FindAttribute("DeltaArg");
	if (attr) attr->QueryFloatValue(&DeltaArg);
	else DeltaArg = 0;

	attr = elem->FindAttribute("HPPercent");
	if (attr) attr->QueryFloatValue(&HPPercent);
	else HPPercent = 100.0f;

	SpawnMonsterId = 0;
	mWaveIdIndex = AINodeFactory::CreateIndexByName("WaveId");
	mUnitIdIndex = AINodeFactory::CreateIndexByName("UnitId");
}

bool CallMonster::Update(Unit* unit)
{
	if (!unit->GetXObject()->AIComponent())
		AIRETURN("ReceiveAIEvent", false);

	DEFINE_AI_DEBUG_HELPER(this, unit);

	AIBehaviorTree* behaviorTree = unit->BehaviorTree();
	SharedData* data = unit->AIData();

	Vector3 monsterPos(0, 0, 0);
	float facearg = data->GetFloatByIndex(mAngleIndex, AngleValue);

	if (BornPosType == 0)
		monsterPos = data->GetVector3ByIndex(mEventPosIndex, EventPos);
	else if (BornPosType == 2)
	{
		monsterPos = pos[0] + behaviorTree->Data().GetVector3ByIndex(mEventPosIndex);
	}
	else
		monsterPos = pos[XRandom::randInt(0, 4)];

	int WaveId = data->GetIntByIndex(mWaveIdIndex);
	const string& oldUnit = data->GetStringByIndex(mUnitIdIndex);
	UINT64 oldUnitId = convert<UINT64>(oldUnit);
	Grid* curGrid = unit->GetCurrScene()->GetGrid();

	if (!curGrid)
		AI_DEBUG_RETURN_WITHOUT_DEFINE("CallMonster", false);

	if (DistValue > 0)
	{
		bool posInMap = false;
		Vector3 orgPos = monsterPos;
		float y = -1;

		for (int i=0; i<10; i++) // 随机10次确保点在地图内
		{
			monsterPos.x += DistValue * ((float)XRandom::randDouble(-1.0f, 1.0f));
			monsterPos.z += DistValue * ((float)XRandom::randDouble(-1.0f, 1.0f));

			if(curGrid->TryGetHeight(monsterPos.x, monsterPos.z, y))
			{
				if (y >= 0)
				{
					posInMap = true;
					break;
				}
			}
		}

		if (posInMap == false)
		{
			if (ForcePlace)
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "Cant Find Proper Pos Cause of XZ; ");
				AI_DEBUG_RETURN_WITHOUT_DEFINE("CallMonster", false);
			}

			if(curGrid->TryGetHeight(orgPos.x, orgPos.z, y))
			{
				if (y >= 0)
				{
					monsterPos = orgPos;
				}
				else
				{
					if (FinalPos != Vector3::zero)
						monsterPos = FinalPos;
					else
					{
						AI_DEBUG_STATEMENT(m_DebugInfo << "Cant Find Proper Pos Cause of Y " << y << "; ");
						AI_DEBUG_RETURN_WITHOUT_DEFINE("CallMonster", false);
					}
				}
			}
		}
	}

	float height = -1;
	if(curGrid->TryGetHeight(monsterPos.x, monsterPos.z, height))
	{
		if (height < 0)
		{
			if (ForcePlace)
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << "Cant Find Proper Pos Cause of Y " << height << "; ");
				AI_DEBUG_RETURN_WITHOUT_DEFINE("CallMonster", false);
			}
			else
			{
				if (FinalPos != Vector3::zero)
					monsterPos = FinalPos;
				else
				{
					AI_DEBUG_STATEMENT(m_DebugInfo << "Cant Find Proper Pos Cause of Y " << height << "; ");
					AI_DEBUG_RETURN_WITHOUT_DEFINE("CallMonster", false);
				}
			}
		}
	}

	int monsterID = data->GetIntByIndex(MonsterIDIndex, MonsterId);
	AI_DEBUG_STATEMENT(m_DebugInfo << monsterID << "; ");

	if (MaxMonsterNum != 0 && unit->GetCurrScene())
	{
		FindEnemyTemplateIdCondition cond(monsterID);
		std::vector<Unit *> units;
		unit->GetCurrScene()->GetUnitByCondition(cond, units);

		if (unit->AIComponent()->IsDebugEnable())
		{
			char infobuf[256]; 
			sprintf(infobuf, "Count monsternumber, mosterid[%d], number[%d]", monsterID, (int)units.size());
			AIOnlineDebugMgr::Instance()->SendDebugMsg(unit->GetCurrScene()->GetSceneID(), infobuf);
		}

		if (units.size() >= (UINT32)MaxMonsterNum)
		{
			if(unit->AIComponent()->IsDebugEnable())
			{
				char infobuf[256]; 
				sprintf(infobuf, "Monsternum full, mosterid[%d], MaxMonsterNum[%d]", monsterID, MaxMonsterNum);
				AIOnlineDebugMgr::Instance()->SendDebugMsg(unit->GetCurrScene()->GetSceneID(), infobuf);
			}

			AI_DEBUG_STATEMENT(m_DebugInfo << "Too Many Monsters " << units.size() << " >= " << MaxMonsterNum << "; ");
			AI_DEBUG_RETURN_WITHOUT_DEFINE("CallMonster", true);
		}
	}

	if (MonsterDelayTime > 0.0f)
	{
		CallMonsterArgs* args = new CallMonsterArgs();
		args->MonsterTemplateId = monsterID;
		args->MonsterPos = monsterPos;
		args->CopyMonsterId = CopyMonsterId;
		args->WaveId = WaveId;
		args->EnableDebug = unit->GetXObject()->AIComponent()->IsDebugEnable();
		args->OldMonsterUnitId = oldUnitId;
		args->SceneID = unit->GetCurrScene()->GetSceneID();
		args->FaceDir = facearg + DeltaArg;
		args->HPPercent = HPPercent;
		args->MonsterUnitId = unit->GetID();

		HTIMER timer = XCommon::SetTimer(MonsterDelayTime, DelegateCombination(CallMonster, LateSpawn, this), args, __FILE__, __LINE__);
		unit->GetXObject()->AIComponent()->AddTimerToken(timer);

		AI_DEBUG_STATEMENT(m_DebugInfo << "Will Late Call at " << args->MonsterPos << " after " << MonsterDelayTime << "s; ");
	}
	else
	{
		Enemy* enemy = EnemyManager::Instance()->CreateEnemy(monsterID, monsterPos, 0.0f);
		AI_DEBUG_STATEMENT(m_DebugInfo << "Will Call at " << monsterPos << "; ");

		if (unit->GetXObject()->AIComponent()->IsDebugEnable())
		{
			char infobuf[256]; 
			sprintf(infobuf, "Will Call Monster[%d], MonsterLife[%f]", monsterID, MonsterLifeTime);
			AIOnlineDebugMgr::Instance()->SendDebugMsg(unit->GetCurrScene()->GetSceneID(), infobuf);
		}

		if (enemy && unit->GetCurrScene())
		{
			if (CopyMonsterId > 0)
			{
				FindEnemyTemplateIdCondition cond(CopyMonsterId);
				std::vector<Unit *> units;
				unit->GetCurrScene()->GetUnitByCondition(cond, units);

				if (units.size() > 0)
				{
					enemy->SetAttr(BASIC_CurrentHP, units[0]->GetAttr(TOTAL_CurrentHP));
					enemy->SetAttr(BASIC_CurrentSuperArmor, units[0]->GetAttr(TOTAL_CurrentSuperArmor));
				}
			}

			if (HPPercent > 0 && HPPercent < 100)
			{
				double fvalue = enemy->GetAttr(BASIC_MaxHP);
				fvalue = fvalue * HPPercent / 100;
				enemy->SetAttr(BASIC_CurrentHP, fvalue);
			}

			if (oldUnitId != 0 && WaveId >= 0)
				unit->GetCurrScene()->ReviveEnemy(enemy, WaveId, oldUnitId);

			enemy->GetXObject()->SetFace(facearg + DeltaArg);
			unit->GetCurrScene()->AddUnitAndNotify(enemy);

			if (unit->GetSecurityStatistics() && unit->GetSecurityStatistics()->_AIInfo)
				unit->GetSecurityStatistics()->_AIInfo->OnCallMonster();

		}
		else
			AI_DEBUG_RETURN_WITHOUT_DEFINE("CallMonster", false);

		if (MonsterLifeTime > 0)
		{
			CallMonsterArgs* args = new CallMonsterArgs();
			args->MonsterUnitId = enemy->GetID();
			HTIMER timer = XCommon::SetTimer(MonsterLifeTime, DelegateCombination(CallMonster, KillSpawn, this), args, __FILE__, __LINE__);
			unit->GetXObject()->AIComponent()->AddTimerToken(timer);
		}

	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("CallMonster", true);
}

bool CallMonster::LateSpawn(IArgs* pargs, void*)
{
	CallMonsterArgs* args = (CallMonsterArgs*)pargs;

	Enemy* enemy = EnemyManager::Instance()->CreateEnemy(args->MonsterTemplateId, args->MonsterPos, 0.0f);
	Scene* scene = SceneManager::Instance()->FindBySceneID(args->SceneID);
	
	if (args->EnableDebug)
		LogInfo("Will Late Call Monster[%d], MonsterLife[%f]", MonsterId, MonsterLifeTime);

	if (enemy && scene)
	{
		if (CopyMonsterId > 0)
		{
			FindEnemyTemplateIdCondition cond(CopyMonsterId);
			std::vector<Unit *> units;
			scene->GetUnitByCondition(cond, units);

			if (units.size() > 0)
			{
				enemy->SetAttr(BASIC_CurrentHP, units[0]->GetAttr(TOTAL_CurrentHP));
				enemy->SetAttr(BASIC_CurrentSuperArmor, units[0]->GetAttr(TOTAL_CurrentSuperArmor));
			}
		}

		if (args->HPPercent > 0 && args->HPPercent < 100)
		{
			double fvalue = enemy->GetAttr(BASIC_MaxHP);
			fvalue = fvalue * HPPercent / 100;
			enemy->SetAttr(BASIC_CurrentHP, fvalue);
		}

		if (args->WaveId != 0 && args->OldMonsterUnitId != 0)
			scene->ReviveEnemy(enemy, args->WaveId, args->OldMonsterUnitId);

		enemy->GetXObject()->SetFace(args->FaceDir);
		scene->AddUnitAndNotify(enemy);

		Unit* unit = Unit::FindUnit(args->MonsterUnitId);

		if (unit != NULL)
		{
			if (unit->GetSecurityStatistics() && unit->GetSecurityStatistics()->_AIInfo)
				unit->GetSecurityStatistics()->_AIInfo->OnCallMonster();
		}
	}
	else
		return false;

	if (MonsterLifeTime > 0)
	{
		CallMonsterArgs* killargs = new CallMonsterArgs();
		killargs->MonsterUnitId = enemy->GetID();
		killargs->EnableDebug = args->EnableDebug;
		HTIMER timerToken = XCommon::SetTimer(MonsterLifeTime, DelegateCombination(CallMonster, KillSpawn, this), killargs, __FILE__, __LINE__);

		if (scene->GetGlobalAI()->GetXObject())
			scene->GetGlobalAI()->GetXObject()->AIComponent()->AddTimerToken(timerToken);
	}

	return true;
}

bool CallMonster::KillSpawn(IArgs* pargs, void*)
{
	CallMonsterArgs* args = (CallMonsterArgs*)pargs;
	Unit* unit = Unit::FindUnit(args->MonsterUnitId);

	if (unit)
	{
		if (args->EnableDebug)
			LogInfo("Unit monster templateid[%d], isdead[%d]", unit->GetTemplateID(), unit->IsDead());

		if (!unit->IsDead())
		{
			unit->TriggerDeath();
		}
	}

	return true;
}

MixMonsterPos::MixMonsterPos(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("MixMonsterId0");
	if (attr) attr->QueryIntValue(&MonsterTemplate0);

	attr = elem->FindAttribute("MixMonsterId1");
	if (attr) attr->QueryIntValue(&MonsterTemplate1);

	attr = elem->FindAttribute("MixMonsterId2");
	if (attr) attr->QueryIntValue(&MonsterTemplate2);
}

bool MixMonsterPos::Update(Unit* unit)
{
	FindEnemyTemplateIdCondition cond0(MonsterTemplate0);
	FindEnemyTemplateIdCondition cond1(MonsterTemplate1);
	FindEnemyTemplateIdCondition cond2(MonsterTemplate2);
	std::vector<Unit *> units0;
	std::vector<Unit *> units1;
	std::vector<Unit *> units2;
	std::vector<Vector3> pos;

	if (MonsterTemplate0 != 0)
		unit->GetCurrScene()->GetUnitByCondition(cond0, units0);

	if (MonsterTemplate1 != 0)
		unit->GetCurrScene()->GetUnitByCondition(cond1, units1);

	if (MonsterTemplate2 != 0)
		unit->GetCurrScene()->GetUnitByCondition(cond2, units2);

	for (UINT32 i=0; i<units1.size(); i++)
		units0.push_back(units1[i]);

	for (UINT32 i=0; i<units2.size(); i++)
		units0.push_back(units2[i]);

	int startIndex = XRandom::randInt(1, units0.size());

	for (UINT32 i=0; i<units0.size(); i++)
		pos.push_back(units0[i]->GetXObject()->GetPosition());

	for (UINT32 i=0; i<units0.size(); i++)
		units0[i]->GetXObject()->SetPosition(pos[(i+startIndex)%units0.size()].x, pos[(i+startIndex)%units0.size()].y, pos[(i+startIndex)%units0.size()].z);

	AI_DEBUG_RETURN("MixMonsterPos", true);
}

KillMonster::KillMonster(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("MonsterId");
	if (attr) attr->QueryIntValue(&MonsterId);
	else MonsterId = 0;

	attr = elem->FindAttribute("DelayTime");
	if (attr) attr->QueryFloatValue(&DelayTime);
	else DelayTime = 0;
}

bool KillMonster::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (DelayTime > 0)
	{
		KillMonsterArgs* killargs = new KillMonsterArgs();
		killargs->MonsterTemplateId = MonsterId;
		killargs->SceneID = unit->GetCurrScene()->GetSceneID();
		HTIMER timerToken = XCommon::SetTimer(DelayTime, DelegateCombination(KillMonster, DelayKillMonster, this), killargs, __FILE__, __LINE__);

		if (unit->GetCurrScene()->GetGlobalAI()->GetXObject())
			unit->GetCurrScene()->GetGlobalAI()->GetXObject()->AIComponent()->AddTimerToken(timerToken);

		AI_DEBUG_STATEMENT(m_DebugInfo << "Will Late Kill After" << DelayTime << "s; ");
	}
	else
	{
		FindEnemyTemplateIdCondition cond0(MonsterId);
		std::vector<Unit *> units0;

		if (MonsterId != 0)
			unit->GetCurrScene()->GetUnitByCondition(cond0, units0);

		if (units0.size() > 0)
		{
			for (UINT32 i=0; i<units0.size(); i++)
			{
				if (!units0[i]->IsDead())
				{
					units0[i]->TriggerDeath();
				}
			}
			AI_DEBUG_STATEMENT(m_DebugInfo << "Kill " << units0.size() << "; ");
		}
	}

	AI_DEBUG_RETURN("KillMonster", true);
}

bool KillMonster::DelayKillMonster(IArgs* args, void*)
{
	KillMonsterArgs* killargs = (KillMonsterArgs*)args;
	FindEnemyTemplateIdCondition cond0(MonsterId);
	std::vector<Unit *> units0;

	Scene* scene = SceneManager::Instance()->FindBySceneID(killargs->SceneID);

	if (MonsterId != 0)
		scene->GetUnitByCondition(cond0, units0);

	if (units0.size() > 0)
	{
		for (UINT32 i=0; i<units0.size(); i++)
		{
			if (!units0[i]->IsDead())
			{
				units0[i]->TriggerDeath();
			}
		}
	}

	return true;
}

AddBuff::AddBuff(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_MonsterIdmValue");
	if (attr) attr->QueryIntValue(&MonsterId);

	attr = elem->FindAttribute("Shared_MonsterIdName");
	MonsterNameIndex = -1;
	if (attr)
	{
		MonsterIdName = attr->Value();
		if (MonsterIdName.length() > 0)
			MonsterNameIndex = AINodeFactory::CreateIndexByName(MonsterIdName);
	}

	attr = elem->FindAttribute("Shared_BuffIdmValue");
	if (attr) attr->QueryIntValue(&BuffId);

	attr = elem->FindAttribute("Shared_BuffIdName");
	BuffIdIndex = -1;
	if (attr)
	{
		BuffIdName = attr->Value();
		if (BuffIdName.length() > 0)
			BuffIdIndex = AINodeFactory::CreateIndexByName(BuffIdName);
	}

	attr = elem->FindAttribute("Shared_BuffId2mValue");
	if (attr) attr->QueryIntValue(&BuffId2);

	attr = elem->FindAttribute("Shared_AddBuffTargetmValue");
	if (attr) attr->QueryIntValue(&AddBuffTarget);

	attr = elem->FindAttribute("Shared_AddBuffWaymValue");
	if (attr) attr->QueryIntValue(&AddBuffWay);

	attr = elem->FindAttribute("Shared_PlayerProfIdmValue");
	if (attr) attr->QueryIntValue(&PlayerProfId);
}

#define SKIP_DEAD_OR_ALIVE(deadOrAlive, it) if (((deadOrAlive) == 1 && (*(it))->IsDead()) || ((deadOrAlive) == 2 && !(*(it))->IsDead())) continue;

bool AddBuff::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);

	std::vector<Unit *> units0;
	int curMonsterId = unit->AIData()->GetIntByIndex(MonsterNameIndex, MonsterId);
	int curBuffId = unit->AIData()->GetIntByIndex(BuffIdIndex, BuffId);

	if (MonsterId == -1) // 给所有人加
	{
		FindEnemyCondition cond;
		unit->GetCurrScene()->GetUnitByCondition(cond, units0);

		const std::list<Role*>& roles = unit->GetCurrScene()->GetAllRoles();
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			units0.push_back((Unit*)(*i));
		}
		AI_DEBUG_STATEMENT(m_DebugInfo << "To All " << units0.size() << " Roles; ");
	}
	else if (MonsterId == -2)  // to target
	{
		UINT64 uid = unit->AIComponent()->GetTarget();
		Unit* role = Unit::FindUnit(uid);

		if (role != NULL)
			units0.push_back(role);

		AI_DEBUG_STATEMENT(m_DebugInfo << "To Target[ " << role->GetID() << ", " << role->GetTemplateID() << "]; ");
	}
	else
	{
		if (AddBuffTarget == 0)
		{
			FindEnemyTemplateIdCondition cond0(curMonsterId);

			if (curMonsterId != 0)
				unit->GetCurrScene()->GetUnitByCondition(cond0, units0);

			AI_DEBUG_STATEMENT(m_DebugInfo << "To " << units0.size() << "Monsters, TempID " << curMonsterId << "; ");
		}
		else if (AddBuffTarget == 1)
		{
			const std::list<Role*>& roles = unit->GetCurrScene()->GetAllRoles();
			if (roles.size() > 0)
			{
				int way = AddBuffWay % 10;
				int deadOrAlive = AddBuffWay / 10;	///> 0: 所有玩家； 1：所有活着； 2：所有死的

				if (way == 0)
				{
					for (auto i = roles.begin(); i != roles.end(); ++i)
					{
						SKIP_DEAD_OR_ALIVE(deadOrAlive, i);
						units0.push_back((Unit*)(*i));
					}
					AI_DEBUG_STATEMENT(m_DebugInfo << "To " << units0.size() << " / " << roles.size() << " Roles  ");
				}
				else if (way == 1) 
				{
					const std::list<Role*>* finalRoles = NULL;
					std::list<Role*> deadOrAliveRoles;

					if (deadOrAlive != 0)
					{
						for(auto it = roles.begin(); it != roles.end(); ++it)
						{
							SKIP_DEAD_OR_ALIVE(deadOrAlive, it);
							deadOrAliveRoles.push_back(*it);
						}
						finalRoles = &deadOrAliveRoles;
					}
					else
					{
						finalRoles = &roles;
					}
					if (finalRoles->size() != 0)
					{
						int offset = XRandom::randInt()% finalRoles->size();
						auto i = finalRoles->begin();
						std::advance(i, offset);
						units0.push_back((Unit*)(*i));

						AI_DEBUG_STATEMENT(m_DebugInfo << "To Random Target[ " << (*i)->GetID() << ", " << (*i)->GetTemplateID() << "] From " << finalRoles->size() << " / " << roles.size() << "Roles; ");
					}
				}
				else if (way == 2 && PlayerProfId != 0)
				{
					int mod = pow(10.0f, (int)log10((float)PlayerProfId) + 1);
					if (mod != 0)
					{
						for (auto i = roles.begin(); i != roles.end(); ++i)
						{
							SKIP_DEAD_OR_ALIVE(deadOrAlive, i);
							Role* role = *i;
							if (role->GetProfession() % mod == PlayerProfId)
							{
								units0.push_back((Unit*)role);
							}
						}
					}
					AI_DEBUG_STATEMENT(m_DebugInfo << "To " << units0.size() << " / " << roles.size() << " Roles  ");
				}
			}
		}
	}

	if (units0.size() > 0)
	{
		XAddBuffActionArgs args;
		args.Caster = unit->GetID();
		args.singleBuff.buffID = curBuffId % 100000;
		args.singleBuff.BuffLevel = (curBuffId / 100000 == 0 ? 1 : curBuffId / 100000);

		AI_DEBUG_STATEMENT(m_DebugInfo << "Add [" << args.singleBuff.buffID << ", " << args.singleBuff.BuffLevel << "] To " << units0.size() << " Targets; ");

		for (UINT32 i=0; i<units0.size(); i++)
		{
			if (!units0[i]->IsDead())
			{
				units0[i]->GetXObject()->DispatchAction(&args);

				if (BuffId2 != 0)
				{
					args.singleBuff.BuffLevel = (BuffId2 / 100000 == 0 ? 1 : BuffId2 / 100000);
					args.singleBuff.buffID = BuffId2%100000;
					units0[i]->GetXObject()->DispatchAction(&args);
				}
			}
		}
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("AddBuff", true);
}

RemoveBuff::RemoveBuff(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("MonsterId");
	if (attr) attr->QueryIntValue(&MonsterId);

	attr = elem->FindAttribute("BuffId");
	if (attr) attr->QueryIntValue(&BuffId);
}

bool RemoveBuff::Update(Unit* unit)
{
	if (MonsterId == -1)
	{
		if (unit->GetCurrScene())
		{
			XRemoveBuffActionArgs args;
			args.Caster = 0;
			args.nBuffId = BuffId;

			const std::list<Role*>& roles = unit->GetCurrScene()->GetAllRoles();
			for (auto i = roles.begin(); i != roles.end(); ++i)
			{
				Role* role = *i;
				if (!role->IsDead())
				{
					role->GetXObject()->DispatchAction(&args);
				}
			}
			AI_DEBUG_STATEMENT(m_DebugInfo << "To All " << roles.size() << " Roles; ");
		}
	}
	else if (MonsterId == -2)  // to target
	{
		UINT64 uid = unit->AIComponent()->GetTarget();
		Unit* pTarget = Unit::FindUnit(uid);

		if (pTarget != NULL)
		{
			XRemoveBuffActionArgs args;
			args.Caster = 0;
			args.nBuffId = BuffId;
			pTarget->GetXObject()->DispatchAction(&args);

			AI_DEBUG_STATEMENT(m_DebugInfo << "To Target [" << pTarget->GetID() << ", " << pTarget->GetTemplateID() << "]; ");
		}
	}
	else
	{
		FindEnemyTemplateIdCondition cond0(MonsterId);
		std::vector<Unit *> units0;

		if (MonsterId != 0)
			unit->GetCurrScene()->GetUnitByCondition(cond0, units0);

		AI_DEBUG_STATEMENT(m_DebugInfo << "To " << units0.size() << "Monsters, TempID " << MonsterId << "; ");

		if (units0.size() > 0)
		{
			XRemoveBuffActionArgs args;
			args.Caster = 0;
			args.nBuffId = BuffId;

			for (UINT32 i=0; i<units0.size(); i++)
			{
				if (!units0[i]->IsDead())
				{
					units0[i]->GetXObject()->DispatchAction(&args);
				}
			}
		}
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("RemoveBuff", true);
}

CallScript::CallScript(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("FuncName");
	if (attr) FuncName = attr->Value();

	attr = elem->FindAttribute("DelayTime");
	if (attr) attr->QueryFloatValue(&DelayTime);
}

bool CallScript::Update(Unit* unit)
{
	if (DelayTime > 0)
	{
		CallScriptArgs* args = new CallScriptArgs();
		args->SceneID = unit->GetCurrScene()->GetSceneID();
		args->CmdString = FuncName;

		HTIMER timer = XCommon::SetTimer(DelayTime, DelegateCombination(CallScript, LateCall, this), args, __FILE__, __LINE__);
		unit->GetXObject()->AIComponent()->AddTimerToken(timer);
	}
	else
	{
		if (unit->GetCurrScene()->GetSpawner())
			unit->GetCurrScene()->GetSpawner()->GetLevelScript()->Execute(FuncName);
	}

	AI_DEBUG_RETURN("CallScript", true);
}

bool CallScript::LateCall(IArgs* pargs, void*)
{
	CallScriptArgs* args = (CallScriptArgs*)pargs;
	Scene* scene = SceneManager::Instance()->FindBySceneID(args->SceneID);

	if (scene && scene->GetSpawner())
		scene->GetSpawner()->GetLevelScript()->Execute(args->CmdString);
	return true;
}

DetectEnemyInRange::DetectEnemyInRange(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("RangeType");
	if (attr)attr->QueryIntValue(&RangeType);

	attr = elem->FindAttribute("Radius");
	if (attr)attr->QueryFloatValue(&Radius);
	Radius *= Radius;

	attr = elem->FindAttribute("Angle");
	if (attr)attr->QueryFloatValue(&Angle);

	attr = elem->FindAttribute("Length");
	if (attr)attr->QueryFloatValue(&Length);

	attr = elem->FindAttribute("Width");
	if (attr)attr->QueryFloatValue(&Width);

	attr = elem->FindAttribute("OffsetLength");
	if (attr)attr->QueryFloatValue(&OffsetLength);

	attr = elem->FindAttribute("MonsterType");
	MonsterType = -1;
	if (attr)
		attr->QueryUnsignedValue(&MonsterType);
	if (MonsterType == 0)
		MonsterType = -1;

	attr = elem->FindAttribute("FightGroupType");
	FightGroupType = 0;
	if (attr)
		attr->QueryIntValue(&FightGroupType);

}

bool DetectEnemyInRange::Update(Unit* unit)
{
	const std::vector<Unit*>* opponent = NULL;

	if (FightGroupType == 0)
		opponent = unit->GetCurrScene()->GetOpponents(unit->GetFightGroup());
	else if (FightGroupType == 1)
		opponent = unit->GetCurrScene()->GetAllys(unit->GetFightGroup());

	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (NULL != opponent)
	{
		const Vector3& unitpos = unit->GetXObject()->GetPosition_p();
		const Vector3& unitface = unit->GetXObject()->GetFace_p();
		Vector3 rightdir = Vector3::HorizontalRotate(unitface, 90);

		for (UINT32 i = 0; i < opponent->size(); ++i)
		{
			Unit* pTarget = (*opponent)[i];

			if (pTarget == unit)
				continue;

			if (MonsterType != -1)
			{
				UINT32 tag = pTarget->GetTag();

				if ((tag & MonsterType) == 0)
					continue;
			}
			const Vector3& oppopos = pTarget->GetXObject()->GetPosition_p();
			if (RangeType == 0)  // circle
			{
				float length = Vector3::sqrtMagnitude(oppopos- (unitpos+unitface*OffsetLength));
				if ( length <= Radius)
				{
					AI_DEBUG_STATEMENT(m_DebugInfo << "[" << pTarget->GetID() << ", " << pTarget->GetTemplateID() << "] at " << oppopos << "; ");
					AI_DEBUG_RETURN_WITHOUT_DEFINE("DetectEnemyInRange", true);
				}
			}
			else if (RangeType == 1) // rect
			{
				float dotres = Vector3::Dot(oppopos-unitpos, unitface);

				if (fabs(dotres) > Width/2)
					continue;

				dotres = Vector3::Dot(oppopos-unitpos, rightdir);
				if (fabs(dotres) > Length/2)
					continue;

				AI_DEBUG_STATEMENT(m_DebugInfo << "[" << pTarget->GetID() << ", " << pTarget->GetTemplateID() << "] at " << oppopos << "; ");
				AI_DEBUG_RETURN_WITHOUT_DEFINE("DetectEnemyInRange", true);

			}
		}
	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("DetectEnemyInRange", false);
}

XHashFunc::XHashFunc(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Input");
	if (attr) HashName = attr->Value();

	attr = elem->FindAttribute("Shared_ResultName");
	if (attr)
	{
		ResName = attr->Value();
		if (ResName.length() > 0)
			ResNameIndex = AINodeFactory::CreateIndexByName(ResName);
		else
			ResNameIndex = 0;
	}

	attr = elem->FindAttribute("Shared_ResultmValue");
	if (attr) attr->QueryIntValue(&ResValue);
}

bool XHashFunc::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);

	SharedData* data = unit->AIData();
	if(data)
	{
		UINT32 v = XCommon::XHash(HashName.c_str());
		data->SetIntByIndex(ResNameIndex, v);

		AI_DEBUG_STATEMENT(m_DebugInfo << v << " <= " << HashName << "; ");

		LogInfo("Will hash, name[%s], value[%u]", HashName.c_str(), XCommon::XHash(HashName.c_str()));
	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("XHashFunc", true);
}

AIDoodad::AIDoodad(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_DoodadIdName");

	doodadIndex = -1;
	if (attr)
	{
		string name(attr->Value());

		if (name.length() > 0)
			doodadIndex = AINodeFactory::CreateIndexByName(name);
	}

	attr = elem->FindAttribute("Shared_DoodadIdmValue");
	if (attr) attr->QueryIntValue(&doodadId);

	attr = elem->FindAttribute("Shared_DoodadPosName");
	posIndex = -1;
	if (attr)
	{
		string name(attr->Value());
		if (name.length() > 0)
			posIndex = AINodeFactory::CreateIndexByName(name);
	}

	attr = elem->FindAttribute("Shared_DoodadPosmValue");
	if (attr) 
	{
		SharedData::ParseVector3FromString(attr->Value(), doodadPos);
	}

	attr = elem->FindAttribute("RandomPos");
	if (attr) attr->QueryFloatValue(&randompos);
	else randompos = 0;

	attr = elem->FindAttribute("DelayTime");
	if (attr) attr->QueryFloatValue(&delaytime);
	else delaytime = 0;
}

bool AIDoodad::Update(Unit* unit)
{
	DEFINE_AI_DEBUG_HELPER(this, unit);

	AIBehaviorTree* behaviorTree = unit->BehaviorTree();
	if (posIndex != -1) doodadPos = behaviorTree->Data().GetVector3ByIndex(posIndex);

	int theDoodadId = behaviorTree->Data().GetIntByIndex(doodadIndex, doodadId);

	AI_DEBUG_STATEMENT(m_DebugInfo << theDoodadId << " at ");

	if (randompos > 0)
	{
		bool posInMap = false;
		Vector3 orgPos = doodadPos;
		float y = -1;
		Grid* curGrid = unit->GetCurrScene()->GetGrid();

		for (int i=0; i<10; i++) // 随机10次确保点在地图内
		{
			doodadPos.x += randompos * ((float)XRandom::randDouble(-1.0f, 1.0f));
			doodadPos.z += randompos * ((float)XRandom::randDouble(-1.0f, 1.0f));

			if(curGrid->TryGetHeight(doodadPos.x, doodadPos.z, y))
			{
				if (y >= 0)
				{
					posInMap = true;
					break;
				}
			}
		}

		if (posInMap == false)
		{
			doodadPos = orgPos;
		}
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << doodadPos << "; ");

	if (delaytime > 0)
	{
		AIDoodadArgs* args = new AIDoodadArgs();
		args->DoodadID = theDoodadId;
		args->WaveID = waveid;
		args->DoodadPos = doodadPos;
		args->SceneId = unit->GetCurrScene()->GetSceneID();
		HTIMER timer = XCommon::SetTimer(delaytime, DelegateCombination(AIDoodad, DelayDoodad, this), args, __FILE__, __LINE__);
		unit->GetXObject()->AIComponent()->AddTimerToken(timer);
	}
	else
		unit->GetCurrScene()->GenerateDoodad(theDoodadId, waveid, doodadPos);
	
	AI_DEBUG_RETURN_WITHOUT_DEFINE("AIDoodad", true);
}

bool AIDoodad::DelayDoodad(IArgs* args, void*)
{
	AIDoodadArgs* doodadargs = (AIDoodadArgs*)args;
	Scene* curScene = SceneManager::Instance()->FindBySceneID(doodadargs->SceneId);

	if (curScene)
		curScene->GenerateDoodad(doodadargs->DoodadID, doodadargs->WaveID, doodadargs->DoodadPos);
	return true;
}

RemoveSceneBuff::RemoveSceneBuff(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("BuffId");
	
	if (attr) attr->QueryIntValue(&doodadid);
}

bool RemoveSceneBuff::Update(Unit* unit)
{
	unit->GetCurrScene()->GetSceneDoodad()->RemoveBuffDoodadById(doodadid);
	AI_DEBUG_RETURN("RemoveSceneBuff", true);
}

RandomEntityPos::RandomEntityPos(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("TemplateId");
	if (attr) attr->QueryIntValue(&TemplateId);

	attr = elem->FindAttribute("Radius");
	if (attr) attr->QueryFloatValue(&Radius);

	attr = elem->FindAttribute("CenterPos");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), CenterPos);

	attr = elem->FindAttribute("FinalPos");
	if (attr) SharedData::ParseVector3FromString(attr->Value(), FinalPos);
	else FinalPos.set(0, 0, 0);

	attr = elem->FindAttribute("NearPlayerTemplateId");
	if (attr) attr->QueryIntValue(&NearPlayerTemplateId);
	else NearPlayerTemplateId = 0;

}

bool RandomEntityPos::Update(Unit* unit)
{
	FindEnemyTemplateIdCondition cond0(TemplateId);
	std::vector<Unit *> units0;
	float y = -1;
	Grid* curGrid = unit->GetCurrScene()->GetGrid();

	if (TemplateId != 0)
		unit->GetCurrScene()->GetUnitByCondition(cond0, units0);
	else
		AI_DEBUG_RETURN("RandomEntityPos", false);

	for (UINT32 i=0; i<units0.size(); i++)
	{
		Vector3 pos = CenterPos;

		if (NearPlayerTemplateId != 0)
		{
			const std::list<Role*>& roles = unit->GetCurrScene()->GetAllRoles();

			///> 只要活的
			std::list<Role*> finalRoles;
			for(auto it = roles.begin(); it != roles.end(); ++it)
			{
				SKIP_DEAD_OR_ALIVE(1, it);
				if (NearPlayerTemplateId > 0 && (*it)->GetProfession() != NearPlayerTemplateId)
					continue;

				finalRoles.push_back(*it);
			}

			if (finalRoles.size() > 0)
			{
				int offset = XRandom::randInt()%finalRoles.size();
				auto i = finalRoles.begin();
				std::advance(i, offset);
				pos = ((Unit*)(*i))->GetXObject()->GetPosition();
			}

			//if (NearPlayerTemplateId == -1)
			//{
			//	
			//	if (roles.size() > 0)
			//	{
			//		int offset = XRandom::randInt()%roles.size();
			//		auto i = roles.begin();
			//		std::advance(i, offset);
			//		CenterPos = ((Unit*)(*i))->GetXObject()->GetPosition();
			//	}
			//}
			//else
			//{
			//	for (auto i = roles.begin(); i != roles.end(); ++i)
			//	{
			//		Role* role = *i;
			//		if (role->GetProfession() == NearPlayerTemplateId)
			//		{
			//			CenterPos = role->GetXObject()->GetPosition();
			//			break;
			//		}
			//	}
			//}

			//pos = CenterPos;
		}

		bool findpos = false;

		Vector3 originPos = pos;

		for (int j=0; j<10; j++)
		{
			float angle = (float)XRandom::randDouble(0, 360);
			float radius = (float)XRandom::randDouble(Radius/3, Radius);


			Vector3 theVec = Vector3::HorizontalRotate(Vector3::forward, angle);
			theVec = theVec*radius;

			pos = originPos + theVec;

			if(curGrid->TryGetHeight(pos.x, pos.z, y))
			{
				if (y >= 0)
				{
					findpos = true;
					break;
				}
			}
		}

		if (findpos == false)
		{
			if (FinalPos != Vector3::zero)
				pos = FinalPos;
			else
				continue;
		}

		units0[i]->GetXObject()->SetPosition_s(pos);
		//units0[i]->GetXObject()->CorrectLocation(pos, units0[i]->GetXObject()->GetFaceDegree());
	}

	AI_DEBUG_RETURN("RandomEntityPos", true);
}

SelectPlayerFromList::SelectPlayerFromList(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("SelectType");
	if (attr) attr->QueryIntValue(&SelectType);
	else SelectType = 0;

	attr = elem->FindAttribute("StartIndex");
	if (attr) attr->QueryIntValue(&StartIndex);
	else StartIndex = 0;

	attr = elem->FindAttribute("EndIndex");
	if (attr) attr->QueryIntValue(&EndIndex);
	else EndIndex = 0;
}

bool SelectPlayerFromList::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();

	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (SelectType == 0)
	{
		SceneHorseRace* pHorseRace = unit->GetCurrScene()->GetHorseRaceHandler();
		if (!pHorseRace) 
			pHorseRace = unit->GetCurrScene()->GetWeekEnd4v4HorseRacingHandler();
		if (!pHorseRace)
			return false;

		std::vector<UINT64> roles;
		pHorseRace->GetRank(roles);

		AI_DEBUG_STATEMENT(m_DebugInfo << roles.size() << "; ");

		if ((int)roles.size() <= StartIndex || StartIndex < 0)
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Too few roles. Failed");
			return false;
		}
		auto it = roles.begin();

		for (int i=0; i<StartIndex; i++)
		{
			if (it != roles.end())
				it ++;
		}

		if (it != roles.end())
		{
#ifdef AIDEBUG
			Unit* pUnit = Unit::FindUnit(*it);
			if (pUnit)
				AI_DEBUG_STATEMENT(m_DebugInfo << "Target " << pUnit->GetID() << ", TempID " << pUnit->GetTemplateID() << "; ");
#endif
			ai->SetTarget(*it);
		}
		else
			AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectPlayerFromList", false);

	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("SelectPlayerFromList", true);
}


GetEntityPos::GetEntityPos(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("IsPlayer");
	if (attr) attr->QueryIntValue(&IsPlayer);
	else IsPlayer = 0;

	attr = elem->FindAttribute("TemplateId");
	if (attr) attr->QueryIntValue(&TemplateId);
	else TemplateId = 0;

	attr = elem->FindAttribute("Shared_StorePosName");
	StorePosIndex = -1;

	if (attr)
	{
		string name(attr->Value());
		if (name.length() > 0)
			StorePosIndex = AINodeFactory::CreateIndexByName(name);
	}

}

bool GetEntityPos::Update(Unit* unit)
{
	AIBehaviorTree* behaviorTree = unit->BehaviorTree();

	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (IsPlayer)
	{
		const std::list<Role*>& roles = unit->GetCurrScene()->GetAllRoles();
		if (TemplateId == -1)
		{

			if (roles.size() > 0)
			{
				int offset = XRandom::randInt()%roles.size();
				auto i = roles.begin();
				std::advance(i, offset);

				AI_DEBUG_STATEMENT(m_DebugInfo << ((Unit*)(*i))->GetXObject()->GetPosition() << ", Rand to [" << (*i)->GetID() << ", " << (*i)->GetTemplateID() << "]; ");
				behaviorTree->Data().SetVector3ByIndex(StorePosIndex, ((Unit*)(*i))->GetXObject()->GetPosition());
			}
		}
		else
		{
			for (auto i = roles.begin(); i != roles.end(); ++i)
			{
				Role* role = *i;
				if (role->GetProfession() == TemplateId)
				{
					AI_DEBUG_STATEMENT(m_DebugInfo << ((Unit*)(*i))->GetXObject()->GetPosition() << ", Rand to [" << (*i)->GetID() << ", " << (*i)->GetTemplateID() << "]; ");
					behaviorTree->Data().SetVector3ByIndex(StorePosIndex, ((Unit*)(*i))->GetXObject()->GetPosition());
					break;
				}
			}
		}
	}
	else
	{
		FindEnemyTemplateIdCondition cond0(TemplateId);
		std::vector<Unit *> units0;

		if (TemplateId != 0)
			unit->GetCurrScene()->GetUnitByCondition(cond0, units0);

		if (units0.size() > 0)
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << units0[0]->GetXObject()->GetPosition() << ", Rand to [" << units0[0]->GetID() << ", " << units0[0]->GetTemplateID() << "]; ");
			behaviorTree->Data().SetVector3ByIndex(StorePosIndex, units0[0]->GetXObject()->GetPosition());
		}
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("GetEntityPos", true);
}

GetEntityCount::GetEntityCount(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Way");
	if (attr) attr->QueryIntValue(&Way);
	else Way = 0;

	attr = elem->FindAttribute("Shared_StoreCountName");
	StoreCountIndex = -1;

	if (attr)
	{
		string name(attr->Value());
		if (name.length() > 0)
			StoreCountIndex = AINodeFactory::CreateIndexByName(name);
	}
}

bool GetEntityCount::Update(Unit* unit)
{
	SharedData* data = unit->AIData();

	if (!data)
		return false;

	DEFINE_AI_DEBUG_HELPER(this, unit);

	int deadOrAlive = Way;	///> 0: 所有玩家； 1：所有活着； 2：所有死的

	int res = 0;

	const std::list<Role*>& roles = unit->GetCurrScene()->GetAllRoles();
	if (roles.size() > 0)
	{
		for (auto i = roles.begin(); i != roles.end(); ++i)
		{
			SKIP_DEAD_OR_ALIVE(deadOrAlive, i);
			++res;
		}
	}
	AI_DEBUG_STATEMENT(m_DebugInfo << "Res " << res << ", Total " << roles.size() << "; ");

	data->SetIntByIndex(StoreCountIndex, res);

	AI_DEBUG_RETURN_WITHOUT_DEFINE("GetEntityCount", true);
}

SetEnmity::SetEnmity(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("OperateType");
	if (attr)
		attr->QueryIntValue((int*)&OperateType);
	else 
		OperateType = Invalid;

	OperateValueIndex = -1;
	attr = elem->FindAttribute("Shared_OperateValueName");
	if (attr)
	{
		std::string name = attr->Value();	

		if (name != "")
			OperateValueIndex = AINodeFactory::CreateIndexByName(name.c_str());
	}

	OperateValue = 0;
	attr = elem->FindAttribute("Shared_OperateValuemValue");
	if (attr)
	{
		OperateValue = attr->FloatValue();
	}
}

bool SetEnmity::Update(Unit* unit)
{
	AIBehaviorTree* behaviorTree = unit->BehaviorTree();
	XAIComponent* ai = unit->AIComponent();

	DEFINE_AI_DEBUG_HELPER(this, unit);

	switch(OperateType)
	{
	case ClearAll:
		{
			AI_DEBUG_STATEMENT(m_DebugInfo << "Clear All Enmity, Not Fighting; Success; ");
			ai->EnmityList().Reset();
			ai->SetFighting(false);
			return true;
		}
		break;
	default:
		break;
	}

	UINT64 target = ai->GetTarget();
	
	Unit* pTarget = Unit::FindUnit(target);
	if (pTarget == NULL)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No Target; Failed; ");
		return false;
	}

	AI_DEBUG_STATEMENT(m_DebugInfo << "Target [" << pTarget->GetID() << ", " << pTarget->GetTemplateID() << "]; ");

	float v = unit->AIData()->GetFloatByIndex(OperateValueIndex, OperateValue);
	switch(OperateType)
	{
	case HateValue:
		ai->EnmityList().AddHateValue(pTarget, v);
		AI_DEBUG_STATEMENT(m_DebugInfo << "Add Enmity " << v << "; ");
		break;
	case Provoke:
		{
			if (v > 0)
			{
				ai->EnmityList().SetProvokeUnit(pTarget, v);
				AI_DEBUG_STATEMENT(m_DebugInfo << "Provoked; ");
			}
			else
			{
				ai->EnmityList().RemoveProvokeUnit(pTarget->GetID());
				AI_DEBUG_STATEMENT(m_DebugInfo << "No Provoked; ");
			}
		}
		break;
	case ProvokeIfNotProvoked:
		{
			if (v > 0)
			{
				AI_DEBUG_STATEMENT(m_DebugInfo << (ai->EnmityList().SetProvokeUnitIfNotProvoked(pTarget, v) ? "Provoked; " : "Not Provoked Cause Already Provoked; "));
			}
			else
			{
				ai->EnmityList().RemoveProvokeUnit(pTarget->GetID());
				AI_DEBUG_STATEMENT(m_DebugInfo << "No Provoked; ");
			}
		}
		break;
	default:
		break;
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("SetEnmity", true);
}

GetMonsterID::GetMonsterID(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Type");
	if (attr)
		attr->QueryIntValue(&MonsterType);
	else 
		MonsterType = 0;

	attr = elem->FindAttribute("Shared_StoreIDName");
	StoreIDIndex = -1;

	if (attr)
	{
		string name(attr->Value());
		if (name.length() > 0)
			StoreIDIndex = AINodeFactory::CreateIndexByName(name);
	}

}

bool GetMonsterID::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();
	SharedData* data = unit->AIData();

	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (!data || !ai || !ai->GetSceneHandler())
		AI_DEBUG_RETURN_WITHOUT_DEFINE("GetMonsterID", false);

	int monsterID = ai->GetSceneHandler()->GetMonsterID(MonsterType);

	AI_DEBUG_STATEMENT(m_DebugInfo << monsterID << ", type: " << MonsterType << "; ");
	
	data->SetIntByIndex(StoreIDIndex, monsterID);
	AI_DEBUG_RETURN_WITHOUT_DEFINE("GetMonsterID", true);
}

GetUID::GetUID(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_StoreIDName");
	StoreIDIndex = -1;

	if (attr)
	{
		string name(attr->Value());
		if (name.length() > 0)
			StoreIDIndex = AINodeFactory::CreateIndexByName(name);
	}
}

bool GetUID::Update(Unit* unit)
{
	SharedData* data = unit->AIData();

	if (!data)
		return false;
	DEFINE_AI_DEBUG_HELPER(this, unit);

	AI_DEBUG_STATEMENT(m_DebugInfo << unit->GetID() << "; ");
	data->SetStringByIndex(StoreIDIndex, ToString(unit->GetID()));
	AI_DEBUG_RETURN_WITHOUT_DEFINE("GetUID", true);
}

AddGroupLevel::AddGroupLevel(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Level");
	Level = 0;

	if (attr)
	{
		attr->QueryIntValue(&Level);
	}

	attr = elem->FindAttribute("Shared_FightGroupName");
	FightGroupIndex = -1;
	if (attr)
	{
		string name(attr->Value());
		if (name.length() > 0)
			FightGroupIndex = AINodeFactory::CreateIndexByName(name);
	}

	attr = elem->FindAttribute("Shared_FightGroupmValue");
	FightGroup = 0;
	if (attr)
	{
		attr->QueryIntValue(&FightGroup);
	}
}

bool AddGroupLevel::Update(Unit* unit)
{
	SharedData* data = unit->AIData();
	SceneMobaBattle* pMobaHandler = unit->GetCurrScene()->GetMobaBattleHandler();

	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (!data || !pMobaHandler)
		AI_DEBUG_RETURN_WITHOUT_DEFINE("AddGroupLevel", false);

	int fightGroup = data->GetIntByIndex(FightGroupIndex, FightGroup);
	if (fightGroup == -1)
	{
		pMobaHandler->AddGroupLevel(unit->GetFightGroup(), (UINT32)Level);
		AI_DEBUG_STATEMENT(m_DebugInfo << "Add " << Level << "To Group " << unit->GetFightGroup() << "; ");
	}
	else
	{
		pMobaHandler->AddGroupLevel(fightGroup, (UINT32)Level);
		AI_DEBUG_STATEMENT(m_DebugInfo << "Add " << Level << "To Group " << fightGroup << "; ");
	}
	AI_DEBUG_RETURN_WITHOUT_DEFINE("AddGroupLevel", true);
}

NotifyTarget::NotifyTarget(const tinyxml2::XMLElement* elem)
{

}

bool NotifyTarget::Update(Unit* unit)
{
	XAIComponent* ai = unit->AIComponent();

	if (!ai)
		return false;

	DEFINE_AI_DEBUG_HELPER(this, unit);

	if (ai->CheckTargetChange())
	{
		PtcG2C_EntityTargetChangeNtf oPtc;
		oPtc.m_Data.set_entityuid(unit->GetID());
		oPtc.m_Data.set_targetuid(ai->GetTarget());
		unit->GetCurrScene()->Broadcast(unit, oPtc);

#ifdef AIDEBUG
		if (debugHelper.ShouldLog())
		{
			m_DebugInfo << "Notify " << ai->GetTarget() << "; ";
			Unit* pUnit = Unit::FindUnit(ai->GetTarget());
			if (pUnit)
				m_DebugInfo << "TemplateID: " << pUnit->GetTemplateID() << "; ";
		}
#endif
	}
	else
	{
#ifdef AIDEBUG
		if (debugHelper.ShouldLog())
		{
			m_DebugInfo << "Not Change " << ai->GetTarget() << "; ";
		}
#endif
	}

	AI_DEBUG_RETURN_WITHOUT_DEFINE("NotifyTarget", true);
}

GetFightGroup::GetFightGroup(const tinyxml2::XMLElement* elem)
{
	const tinyxml2::XMLAttribute * attr = elem->FindAttribute("Shared_UIDName");
	UIDIndex = -1;
	if (attr)
	{
		string name(attr->Value());
		if (name.length() > 0)
			UIDIndex = AINodeFactory::CreateIndexByName(name);
	}

	attr = elem->FindAttribute("Shared_StoreFightGroupName");
	StoreFightGroupIndex = -1;
	if (attr)
	{
		string name(attr->Value());
		if (name.length() > 0)
			StoreFightGroupIndex = AINodeFactory::CreateIndexByName(name);
	}
}

bool GetFightGroup::Update(Unit* unit)
{
	SharedData* data = unit->AIData();
	if (!data)
		return false;

	DEFINE_AI_DEBUG_HELPER(this, unit);

	const string& strUID = data->GetStringByIndex(UIDIndex);
	if (strUID.length() == 0)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "No UID; Failed; ");
		return false;
	}

	UINT64 uid = convert<UINT64>(strUID);
	Unit* pUnit = Unit::FindUnit(uid);
	if (!pUnit)
	{
		AI_DEBUG_STATEMENT(m_DebugInfo << "UID " << strUID <<  " Not Exist; Failed; ");
		return false;
	}
	data->SetIntByIndex(StoreFightGroupIndex, pUnit->GetFightGroup());
	AI_DEBUG_STATEMENT(m_DebugInfo << pUnit->GetFightGroup() << "; Target [" << uid << ", " << pUnit->GetTemplateID() << "]; ");

	AI_DEBUG_RETURN_WITHOUT_DEFINE("GetFightGroup", true);
}
