#include "pch.h"
#include "matchhandler.h"
#include "define/teamdef.h"
#include "herobattlerecord.h"
#include "unit/role.h"
#include "weekend4v4mgr.h"
#include "pkrecord.h"

std::unordered_map<int, MHBase*> MatchHandler::m_subHandler;

KKSG::ErrorCode MatchHandler::RoleCondition(Role* pRole, int type, KKSG::KMatchRole* krole /*= NULL*/)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}

	if(krole)
	{
		krole->set_roleid(pRole->GetID());//必要信息
	}

	KKSG::ErrorCode ret = KKSG::ERR_SUCCESS;

	auto pSub = GetSubHandler(type);
	if(pSub)
	{
		ret = pSub->MatchCondition(pRole);
		TJUDGERET(ret)

		if(krole)
		{
			ret = pSub->FillMatchData(pRole, krole);	
			TJUDGERET(ret)
		}
	}
	else
	{
		;//Get不到表示不需要处理
	}
	return ret;
}

MHBase* MatchHandler::GetSubHandler(int type)
{
	auto it = m_subHandler.find(type);
	if(it != m_subHandler.end())
	{
		return it->second;
	}
	return NULL;
}

KKSG::ErrorCode MHHero::FillMatchData(Role* pRole, KKSG::KMatchRole* krole)
{
	krole->set_elopoint(pRole->Get<CHeroBattleRecord>()->GetELOPoint());
	return KKSG::ERR_SUCCESS;
}

MHHero MHHero::GlobalMHHero;

KKSG::ErrorCode MHWeekendAct::MatchCondition(Role* pRole)
{
	if(!WeekEnd4v4Mgr::Instance()->CheckSystemOpen(pRole))
	{
		return KKSG::ERR_ACT_NOT_OPEN;
	}
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode MHWeekendAct::FillMatchData(Role* pRole, KKSG::KMatchRole* krole)
{
	krole->set_mapid(WeekEnd4v4Mgr::Instance()->GetThisWeekEndSceneID());			
	return KKSG::ERR_SUCCESS;
}

MHWeekendAct MHWeekendAct::GlobalMHWeekendAct;

KKSG::ErrorCode MHPkTwo::MatchCondition(Role* pRole)
{
	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_PKTWO))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode MHPkTwo::FillMatchData(Role* pRole, KKSG::KMatchRole* krole)
{
	krole->mutable_pkrec()->set_point(pRole->Get<CPkRecord>()->GetPoint(KKSG::PK_2v2));
	return KKSG::ERR_SUCCESS;
}

MHPkTwo MHPkTwo::GlobalMHPkTwo;

MHBase::MHBase(int type)
{
	MatchHandler::m_subHandler[type] = this;
}

KKSG::ErrorCode MHBase::MatchCondition(Role* pRole)
{
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode MHBase::FillMatchData(Role* pRole, KKSG::KMatchRole* krole)
{
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode MHSurvive::MatchCondition(Role* pRole)
{
	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_SURVIVE));
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	return KKSG::ERR_SUCCESS;	
}

KKSG::ErrorCode MHSurvive::FillMatchData(Role* pRole, KKSG::KMatchRole* krole)
{
	return KKSG::ERR_SUCCESS;
}

MHSurvive MHSurvive::GlobalMHSurvive;
