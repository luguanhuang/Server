#include "pch.h"
#include "ainodefactory.h"
#include "ainode.h"
#include "ainodelogic.h"
#include "ainodecondition.h"
#include "ainodeaction.h"
#include "ainodetarget.h"
#include "sysnode.h"
#include "ainodemove.h"
#include "ainodeskill.h"
#include "ainodetarget.h"

//使用要求 类名和字符串完全一样
#define NEWNODE(name) else if (#name == nodeName){return new name(elem);}

AINodeBase* AINodeFactory::CreateAINodeByName(const std::string& nodeName, tinyxml2::XMLElement* elem, AINodeBase* root  )
{

	if("Selector" == nodeName)
	{
		return new SelectorNode;
	}
	else if("Sequence" == nodeName)
	{
		return new SequenceNode;
	}
	else if("RandomSelector" == nodeName)
	{
		return new RandomSelectorNode;
	}
	else if("RandomSequence" == nodeName)
	{
		return new RandomSequenceNode;
	}
	else if("Inverter" == nodeName)
	{
		return new Inverter;
	}
	else if("EntryTask" == nodeName)
	{
		return new EntryTaskNode(elem);
	}
	else if("ReturnSuccess" == nodeName)
	{
		return new ReturnSuccess;
	}
	else if("ReturnFailure" == nodeName)
	{
		return new ReturnFailure;
	}
	NEWNODE(ConditionalEvaluator)
	NEWNODE(Log)
	NEWNODE(RandomFloat)
	NEWNODE(FloatOperator)
	NEWNODE(FloatComparison)
	NEWNODE(SetFloat)
	NEWNODE(SetBool)
	NEWNODE(CompareTo)
	NEWNODE(SetInt)
	NEWNODE(IntComparison)
	NEWNODE(IntOperator)
	NEWNODE(BoolComparison)
	NEWNODE(ValueTarget)
	NEWNODE(ValueDistance)
	NEWNODE(ValueHP)
	NEWNODE(ValueMP)
	NEWNODE(ValueFP)
	NEWNODE(StatusBehit)
	NEWNODE(StatusDeath)
	NEWNODE(StatusFreeze)
	NEWNODE(StatusIdle)
	NEWNODE(StatusMove)
	NEWNODE(StatusRotate)
	NEWNODE(StatusSkill)
	NEWNODE(StatusWoozy)
	NEWNODE(NavToTarget)
	NEWNODE(ActionRotate)
	NEWNODE(RotateToTarget)
	NEWNODE(PhysicalAttack)
	NEWNODE(FindTargetByDistance)
	NEWNODE(TargetByHatredList)
	NEWNODE(SelectNonHartedList)
	NEWNODE(TryCastQTE)
	NEWNODE(CastDash)
	NEWNODE(IsOppoCastingSkill)
	NEWNODE(IsHurtOppo)
	NEWNODE(IsFixedInCd)
	NEWNODE(IsWander)
	NEWNODE(IsCastingSkill)
	NEWNODE(IsFixedInCd)
	NEWNODE(IsQTEState)
	NEWNODE(DetectEnimyInSight)
	NEWNODE(FindTargetByHitLevel)
	NEWNODE(IsFighting)
	NEWNODE(DoSelectNearest)
	NEWNODE(DoSelectFarthest)
	NEWNODE(FilterSkill)
	NEWNODE(DoSelectSkillInOrder)
	NEWNODE(DoSelectSkillRandom)
	NEWNODE(DoCastSkill)
	NEWNODE(SetDest)
	NEWNODE(ActionMove)
	NEWNODE(IsSkillChoosed)
	NEWNODE(FindNavPath)
	NEWNODE(ReceiveAIEvent)
	NEWNODE(SendAIEvent)
	NEWNODE(SelectMoveTargetById)
	NEWNODE(SelectItemTarget)
	NEWNODE(SelectBuffTarget)
	NEWNODE(TargetQTEState)
	NEWNODE(ResetTargets)
	NEWNODE(CallMonster)
	NEWNODE(MixMonsterPos)
	NEWNODE(KillMonster)
	NEWNODE(ConditionMonsterNum)
	NEWNODE(ConditionPlayerNum)
	NEWNODE(AddBuff)
	NEWNODE(RemoveBuff)
	NEWNODE(CallScript)
	NEWNODE(IsTargetImmortal)
	NEWNODE(DetectEnemyInRange)
	NEWNODE(XHashFunc)
	NEWNODE(StopCastingSkill)
	NEWNODE(AIDoodad)
	NEWNODE(RemoveSceneBuff)
	NEWNODE(RandomEntityPos)
	NEWNODE(SelectTargetBySkillCircle)
	NEWNODE(CalDistance)
	NEWNODE(ConditionCanReach)
	NEWNODE(Navigation)
	NEWNODE(MoveStratage)
	NEWNODE(GetRealtimeSinceStartup)
	NEWNODE(ResetHartedList)
	NEWNODE(SelectPlayerFromList)
	NEWNODE(GetEntityPos)
	NEWNODE(SetEnmity)
	NEWNODE(DoSelectByUID)
	NEWNODE(DoSelectRoleByIndex)
	NEWNODE(GetMonsterID)
	NEWNODE(AddGroupLevel)
	NEWNODE(GetUID)
	NEWNODE(NotifyTarget)
	NEWNODE(GetFightGroup)
	NEWNODE(DoSelectRandomTarget)
	NEWNODE(GetEntityCount)

	if (nodeName != "PlayFx")
		SSError << "not find right class : " << nodeName.c_str() << END;

	return NULL;
}

AINodeFactory::NameIndexInfo* AINodeFactory::mpCurActiveNameIndexInfo = NULL;
AINodeFactory::NameIndexInfo AINodeFactory::mCommonNameIndexInfo;
std::string AINodeFactory::mCurActiveTreeName;
AINodeFactory::NameIndexInfo AINodeFactory::mTempNameIndexInfo;

int AINodeFactory::CreateIndexByName(const std::string& name)
{
	auto it = mCommonNameIndexInfo.mNameHash.find(name);
	if (it != mCommonNameIndexInfo.mNameHash.end())
		return it->second;

	if (mpCurActiveNameIndexInfo == NULL)
		return -1;

	///> common已经找过，不用再找一遍
	if (mpCurActiveNameIndexInfo == &mCommonNameIndexInfo || mpCurActiveNameIndexInfo->mNameHash.find(name) == mpCurActiveNameIndexInfo->mNameHash.end())
	{
		LogInfo("ADD node: %s, index: %d", name.c_str(), mpCurActiveNameIndexInfo->mNameIndex);
		mpCurActiveNameIndexInfo->mNameHash[name] = mpCurActiveNameIndexInfo->mNameIndex;
		mpCurActiveNameIndexInfo->mNameIndex ++;

		CHECK_COND_WITH_LOG_RETURN(
			mpCurActiveNameIndexInfo->mNameIndex < MAX_COUNT_NUM,
			LogError("Variables count %d out of range %d in %s node of %s", mpCurActiveNameIndexInfo->mNameIndex, MAX_COUNT_NUM, name.c_str(), mCurActiveTreeName.c_str());,
			-1
		);

	}

	return mpCurActiveNameIndexInfo->mNameHash[name];
}

void AINodeFactory::SetActiveTree(const std::string& tree)
{
	LogInfo("SetActiveTree: %s", tree.c_str());
	
	mCurActiveTreeName = tree;

	if (tree.empty())
	{
		mpCurActiveNameIndexInfo = &mCommonNameIndexInfo;
		return;
	}
	else
	{
		mpCurActiveNameIndexInfo = &mTempNameIndexInfo;
	}

	mpCurActiveNameIndexInfo->Reset();
	mpCurActiveNameIndexInfo->mNameIndex = mCommonNameIndexInfo.mNameIndex;
}
