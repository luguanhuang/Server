#include "pch.h"
#include "aiscenehandler.h"
#include "../unit/unit.h"
#include "../component/XAIComponent.h"
#include "../entity/XEntity.h"
#include "../scene/scene.h"
#include "../../protocol/pb/enum.pb.h"
#include "scene/scenemobabattle.h"
#include "aiconfig.h"
#include "gamelogic/mobabattlemgr.h"

int MobaGetMonsterID(AISceneHandler& handler, int type)
{
	///> 根据自己的FightGroup，得知是红蓝方，然后获取当前兵线等级，并从表中拿到对应type的ID
	SceneMobaBattle* pSceneHandler = handler.GetScene()->GetMobaBattleHandler();
	if (pSceneHandler == NULL)
		return 0;
	UINT32 fightGroup = handler.GetEntity()->GetUnit()->GetFightGroup();

	UINT32 level = pSceneHandler->GetGroupLevel(fightGroup);

	return MobaBattleMgr::Instance()->GetMobID(fightGroup, level, type);
}

void NoInitHateValue(AISceneHandler& handler, Unit* pTarget)
{
	///> Just an empty function
}


void MobaInitHate(AISceneHandler& handler, Unit* pTarget)
{
	UINT32 initHate = AIConfig::Instance()->GetHateValue(pTarget->GetTag() & Moba_Tag_TypeAll, handler.GetEntity()->GetUnit()->GetTag() & Moba_Tag_TypeAll);
	if (initHate == 0)
		return;

	XEnmityList& pEnmityList = handler.GetAIComponent()->EnmityList();
	pEnmityList.AddInitHateValue(pTarget, (float)initHate);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

AISceneHandler* AISceneHandler::CreateSceneHandler(XAIComponent* pAIComponent, Scene* pScene)
{
	if (pAIComponent == NULL || pScene == NULL)
		return NULL;

	Unit* pUnit = pAIComponent->GetEntity()->GetUnit();
	if (pUnit == NULL)
		return NULL;

	UINT32 sceneType = pScene->GetSceneType();

	AISceneHandler* pHandler = new AISceneHandler(pAIComponent, pScene);
	switch((KKSG::SceneType)sceneType)
	{
	case KKSG::SCENE_MOBA:
//	case KKSG::SCENE_BATTLE:
		pHandler->_SetMobaFunc();
		break;
	default:
		break;
	}

	return pHandler;
}

AISceneHandler::AISceneHandler(XAIComponent* pAIComponent, Scene* pScene)
{
	m_pAIComponent = pAIComponent;
	m_pEntity = m_pAIComponent->GetEntity();
	m_pScene = pScene;

	_SetDefaultFunc();
}

void AISceneHandler::_SetDefaultFunc()
{
	_GetMonsterID = NULL;
	_OnEnterFight = NULL;
	_OnEnemyDetected = NULL;
}

void AISceneHandler::_SetMobaFunc()
{
	_GetMonsterID = MobaGetMonsterID;
	_OnEnterFight = NoInitHateValue;
	_OnEnemyDetected = MobaInitHate;
}

int AISceneHandler::GetMonsterID(int type)
{
	if (_GetMonsterID) 
		return _GetMonsterID(*this, type); 
	return 0;
}

void AISceneHandler::OnEnterFight(Unit* pTarget)
{
	if (_OnEnterFight)
	{
		_OnEnterFight(*this, pTarget);
		return;
	}

	GetAIComponent()->EnmityList().AddInitHateValue(pTarget);

}

void AISceneHandler::OnEnemyDetected(Unit* pTarget)
{
	if (_OnEnemyDetected)
	{
		_OnEnemyDetected(*this, pTarget);
		return;
	}

	//GetAIComponent()->NotifyAllyIntoFight(pTarget);
}
