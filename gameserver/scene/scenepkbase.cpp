#include "pch.h"
#include "scenepkbase.h"
#include "gamelogic/pkmgr.h"
#include "scene/scene.h"
#include "pk/rpcg2m_pkrankreq.h"
#include "gamelogic/pkrecord.h"
#include "scenestatistics.h"
#include "gamelogic/robotconfig.h"
#include "unit/dummyrole.h"
#include "attributepk.h"
#include "unit/rolemanager.h"

ScenePkBase::ScenePkBase(Scene* scene)
	:SceneVsBase(scene)
{
	m_rankUpdateDelay = 0;
	m_fightUpdateTime = 150;
	if(scene->GetSceneInfo() && scene->GetSceneInfo()->m_pConf && scene->GetSceneInfo()->m_pConf->TimeCounter.size() >= 2)
	{
		m_fightUpdateTime = scene->GetSceneInfo()->m_pConf->TimeCounter[1];
	}
}

ScenePkBase::~ScenePkBase()
{

}

void ScenePkBase::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

void ScenePkBase::Update()
{
	if(GetCurrentStateType() == VS_STATE_END)
	{
		CheckSetEnd();
	}
	else
	{
		m_statemgr.Update();
	}
}

void ScenePkBase::CheckSetEnd()
{
	if(IsEnd())
	{
		return;
	}

	if(0 == m_rankUpdateDelay || GetStateStartTime() + 10 < time(NULL))
	{
		SetEnd();
	}

}

void ScenePkBase::BeginRankUpdateDelay()
{
	std::vector<UINT64> unitIDs;
	FillUnitIDs(unitIDs);
	for(size_t i = 0; i < unitIDs.size(); ++i)
	{
		//排名升降有问题 TODO
		UINT64 unitID = unitIDs[i];
		Role* pRole = m_scene->FindSceneRoleCond(unitID, true, false);
		if(pRole)
		{
			SendRankReq(pRole, m_scene->GetSceneID());
			++m_rankUpdateDelay;
		}
	}
}

void ScenePkBase::EndRankUpdateDelay(const KKSG::QMHRPkRes& res)
{
	--m_rankUpdateDelay;

	UINT32 allrank = res.allrank();
	SSDebug << "allrank = " << allrank << END;
	UINT64 roleid = res.roleid();
	UINT32 lastrank = res.lastrank();
	UINT32 currank = res.currank();
	SSDebug << " rolseid = " << roleid << " lastrank = " << lastrank << " currank = " << currank << END;

	BPkRoleData* spkr = GetRoleBaseData(roleid);
	if(spkr)
	{
		spkr->lastRank = lastrank;
		spkr->curRank = currank;
		spkr->allRank = allrank;
		// scene statistics mark
		if (NULL != m_scene->Statistics() && GetPkNVNType() == KKSG::PK_1v1)
		{
			m_scene->Statistics()->SetPkBeginRank(roleid, lastrank);
			m_scene->Statistics()->SetPkEndRank(roleid, spkr->point, currank);
		}
	}
	else
	{
		SSError << " do not find role spkresult roleid = " << roleid << END;
	}
}

UINT32 ScenePkBase::GetLoadWaitTime()
{
	return GetGlobalConfig().PkLoadWaitTime;
}

void ScenePkBase::FillResult(KKSG::PkResult* result, Role* role)
{
	if(NULL == result || NULL == role)
	{
		return;
	}
	//默认值
	result->set_mystate(KKSG::KK_VS_ROLE_NORMAL);
	result->set_opstate(KKSG::KK_VS_ROLE_NORMAL);

	auto pData = GetRoleBaseData(role->GetID());
	if(NULL == pData)
	{
		result->set_result(KKSG::PkResult_Win);
		result->set_winpoint(0);
		result->set_honorpoint(0);
		result->set_dragoncount(0);
		result->set_rank(0);

		return;
	}

	UINT32 lastrank = pData->lastRank;
	UINT32 currentrank = pData->curRank; 
	UINT32 allrank = pData->allRank;

	result->set_result(pData->result);
	result->set_winpoint(pData->point);
	result->set_honorpoint(pData->honorpoint);
	result->set_dragoncount(pData->dragoncount);


	if (INVALID_RANK == lastrank && INVALID_RANK == currentrank)
	{
		result->set_rank(0);
	}
	else if (INVALID_RANK  == lastrank)
	{
		result->set_rank(allrank - currentrank);
		result->set_firstrank(currentrank);//第一次打
	}
	else if (INVALID_RANK == currentrank)
	{
		result->set_rank(lastrank - allrank);
	}
	else
	{
		result->set_rank(lastrank - currentrank);
	}
}

UINT32 ScenePkBase::GetRobotPoint(UINT32 myPoint)
{
	UINT32 point = myPoint;
	UINT32 random = XRandom::randInt(1, 101);
	if (random >= 50)
	{
		random = 100 - random;
		point += (random/10);
	}
	else
	{
		UINT32 temppoint = (random/10);
		point = point <= temppoint ? 0 : point - temppoint;
	}
	return point;
}

bool ScenePkBase::AddRobot(const KKSG::RoleSmallInfo& roleInfo, int lookupID, KKSG::RoleSmallInfo& robotInfo)
{
	FilterRoleData filter;
	filter.qwRoleID = roleInfo.roleid();
	filter.level = roleInfo.rolelevel();
	filter.ppt = roleInfo.roleppt();
	filter.prof = roleInfo.roleprofession();

	DummyRole* pDummy = CreateRobot(filter, lookupID, 0, ARENA_ENEMY);			
	if(NULL == pDummy)
	{
		SSError << " create robot failed sceneid = " << GetScene()->GetSceneID() << " lookupid = " << lookupID << END;
		return false;
	}

	//公平属性
	AttributePkMgr::Instance()->SetPkAttribute(pDummy, GetScene()->GetSceneType());

	robotInfo.set_roleid(pDummy->GetID());
	robotInfo.set_rolename(pDummy->GetName());
	robotInfo.set_rolelevel(pDummy->GetLevel());
	robotInfo.set_roleprofession(pDummy->GetProfession());
	robotInfo.set_roleppt(pDummy->GetAttr(TOTAL_POWERPOINT));

	return true;
}

void ScenePkBase::SendRankReq(Role* pRole, UINT32 sceneID)
{
	if(NULL == pRole)
	{
		return;
	}
	RpcG2M_PkRankReq* rpc = RpcG2M_PkRankReq::CreateRpc();
	KKSG::QMHRPkArg& arg = rpc->m_oArg;
	arg.set_type(GetPkNVNType());
	arg.set_sceneuniid(sceneID);

	pRole->Get<CPkRecord>()->FillRankData(*arg.mutable_rankdata(), GetPkNVNType());

	pRole->SendToMS(*rpc);
}

//////////////////////////////////////////////////////////////////////////////////////////
void SPkResult::End(KKSG::PkResultType _result)
{
	result = _result;
	point = PkMgr::Instance()->GetPoint(basepoint, other.pointspan, _result);
}
