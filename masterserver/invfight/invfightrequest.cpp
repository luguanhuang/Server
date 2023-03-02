#include "pch.h"
#include "invfightrequest.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "invfightmgr.h"
#include "table/globalconfig.h"
#include "invfight/ptcm2c_invfightnotify.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "invfight/rpcc2m_invfightreqall.h"
#include "invfight/rpcm2g_invfightm2greq.h"
#include "network/gslink.h"
#include "invfight/invfightcrossmgr.h"

InvFightRequest::InvFightRequest(CRole* role)
{
	m_pRole = role;
}

InvFightRequest::~InvFightRequest()
{

}

int InvFightRequest::InvOne(UINT64 roleID, KKSG::InvFightM2GRes& m2gRes)
{
	CRole* pToRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pToRole)
	{
		return KKSG::ERR_INVFIGHT_ROLE_LOGOUT;
	}

	if(!SceneCondition(m_pRole))//先判断场景
	{
		return KKSG::ERR_INVFIGHT_ME_SCENE;
	}
	if(m2gRes.fromlevel() < GetGlobalConfig().InvFightRoleLevel)
	{
		return KKSG::ERR_INVFIGHT_ME_LEVEL;
	}

	if(!SceneCondition(pToRole))
	{
		return KKSG::ERR_INVFIGHT_OTHER_SCENE;
	}
	if(m2gRes.tolevel() < GetGlobalConfig().InvFightRoleLevel)
	{
		return KKSG::ERR_INVFIGHT_OTHER_LEVEL;
	}

	if(InvFightMgr::Instance()->HaveInvTo(m_pRole->GetID(), pToRole->GetID()))
	{
		return KKSG::ERR_INVFIGHT_INV_REPEAT;
	}

	if(InvFightMgr::Instance()->InvFromCount(m_pRole->GetID()) >= GetGlobalConfig().InvFightFromCount)
	{
		return KKSG::ERR_INVFIGHT_INV_COUNT_MAX;
	}

	UINT32 CurInvFightToCount = InvFightMgr::Instance()->InvToCount(pToRole->GetID());
	if(CurInvFightToCount >= InvFightToCountMax)
	{
		return KKSG::ERR_INVFIGHT_INV_TO_COUNT_MAX;
	}

	InvFightMgr::Instance()->MakeInvFight(m_pRole->GetID(), pToRole->GetID());
	
	//notify
	PtcM2C_InvFightNotify ptc;
	ptc.m_Data.set_ntftype(KKSG::IFNT_INVITE_ME);
	ptc.m_Data.set_count(CurInvFightToCount + 1);
	pToRole->Send(ptc);

	return KKSG::ERR_SUCCESS;
}

int InvFightRequest::RefushOne(UINT64 invID)
{
	auto pUniRec = InvFightMgr::Instance()->GetInvUniRec(invID);
	if(NULL == pUniRec)//已过期
	{
		return KKSG::ERR_SUCCESS;
	}

	if(pUniRec->InvToID() != m_pRole->GetID())
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 fromID = pUniRec->InvFromID();

	InvFightMgr::Instance()->Del(invID);

	//from notify
	CRole* pFromRole = CRoleManager::Instance()->GetByRoleID(fromID);
	if(pFromRole && SceneCondition(pFromRole))
	{
		PtcM2C_InvFightNotify ptc;
		ptc.m_Data.set_ntftype(KKSG::IFNT_REFUSE_ME);
		ptc.m_Data.set_name(m_pRole->GetName());
		pFromRole->Send(ptc);
	}
	
	return KKSG::ERR_SUCCESS;
}

int InvFightRequest::IgnoreAll()
{
	InvFightMgr::Instance()->DelToIDInv(m_pRole->GetID());

	InvFightCrossMgr::Instance()->DelToIDInv(m_pRole->GetAccount());

	return KKSG::ERR_SUCCESS;
}

int InvFightRequest::ReqList(KKSG::InvFightRes& roRes)
{
	InvFightMgr::Instance()->FillInvToList(m_pRole->GetID(), roRes);

	InvFightCrossMgr::Instance()->FillInvToList(m_pRole->GetAccount(), roRes);

	if(roRes.roles_size() <= 0)
	{
		return KKSG::ERR_INVFIGHT_INV_TIME_OVER;
	}

	return KKSG::ERR_SUCCESS;
}

int InvFightRequest::AcceptPre(UINT64 invID, UINT64& fromRoleID)
{
	auto pUniRec = InvFightMgr::Instance()->GetInvUniRec(invID);
	if(NULL == pUniRec)
	{
		return KKSG::ERR_INVFIGHT_INV_TIME_OVER;
	}
	fromRoleID = pUniRec->InvFromID();
	return KKSG::ERR_SUCCESS;
}

int InvFightRequest::AcceptOne(UINT64 invID)
{
	auto pUniRec = InvFightMgr::Instance()->GetInvUniRec(invID);
	if(NULL == pUniRec)
	{
		return KKSG::ERR_INVFIGHT_INV_TIME_OVER;
	}
	if(pUniRec->InvToID() != m_pRole->GetID())
	{
		return KKSG::ERR_FAILED;
	}
	CRole* pFromRole = CRoleManager::Instance()->GetByRoleID(pUniRec->InvFromID());
	if(NULL == pFromRole)
	{
		return KKSG::ERR_INVFIGHT_ROLE_LOGOUT;
	}

	if(!SceneCondition(m_pRole))
	{
		return KKSG::ERR_INVFIGHT_ME_SCENE;
	}
	if(!SceneCondition(pFromRole))
	{
		return KKSG::ERR_INVFIGHT_OTHER_SCENE;
	}
	if(InvFightMgr::Instance()->IsInDelay(pUniRec->InvFromID()))
	{
		return KKSG::ERR_INVFIGHT_OTHER_SCENE;
	}

	InvFightMgr::Instance()->Del(invID);
	pUniRec = NULL;

	InvFightMgr::Instance()->ReadyFight(pFromRole->GetID(), m_pRole->GetID());

	return KKSG::ERR_SUCCESS;
}

bool InvFightRequest::ProcessCall(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, InvFightProcessDelayInfo& delayInfo)
{
	bool needDelay = false;
	int ret = int(KKSG::ERR_SUCCESS);
	bool gotogs = false;
	UINT64 fromRoleID = 0;
	switch(roArg.reqtype())
	{
	case IFRT_INV_ONE:
		{
			gotogs = true;
		}
		break;
	case IFRT_REFUSH_ONE:
		{
			ret = RefushOne(roArg.invid());
		}
		break;
	case IFRT_IGNORE_ALL:
		{
			ret = IgnoreAll();
		}
		break;
	case IFRT_REQ_LIST:
		{
			ret = ReqList(roRes);
		}
		break;
	case IFRT_ACCEPT_ONE:
		{
			ret = AcceptPre(roArg.invid(), fromRoleID);
			gotogs = true;
		}
		break;
	default:
		break;
	}
	
	if(ERR_SUCCESS != ret || false == gotogs)
	{
		if(ErrorCode_IsValid(ret))
		{
			roRes.set_errorcode((ErrorCode)ret);
		}
		else
		{
			roRes.set_errorcode(ERR_FAILED);
		}
	}
	else
	{
		needDelay = true;
	}

	if (needDelay)
	{
		delayInfo.fromID = fromRoleID;	
	}

	return needDelay;
}

void InvFightRequest::ProcessDelay(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		roRes.set_errorcode(ERR_FAILED);
		return ;
	}
	InvFightM2GRes* m2gRes = (InvFightM2GRes*)(roUserData.m_pUserPtr);

	if(ERR_SUCCESS != m2gRes->errorcode())
	{
		roRes.set_errorcode(m2gRes->errorcode());
		return ;
	}

	if(InvFightMgr::Instance()->IsInDelay(m_pRole->GetID()))
	{
		roRes.set_errorcode(ERR_INVFIGHT_INV_DELAY);
		return ;
	}
	//InvFightRequest req(pRole);	
	int ret = int(ERR_SUCCESS);
	switch(roArg.reqtype())
	{
	case IFRT_INV_ONE:
		{
			ret = InvOne(roArg.roleid(), *m2gRes);
		}
		break;
	case IFRT_ACCEPT_ONE:
		{
			ret = AcceptOne(roArg.invid());
		}
		break;
	default:
		break;
	}
	
	if(ErrorCode_IsValid(ret))
	{
		roRes.set_errorcode((ErrorCode)ret);
	}
	else
	{
		roRes.set_errorcode(ERR_FAILED);
	}
}

//
//bool InvFightRequest::LevelCondition(CRole* pRole)
//{
//	if(NULL == pRole)
//	{
//		return false;
//	}
//
//	CRoleSummary* pRoleSummry = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
//	if(NULL == pRoleSummry)
//	{
//		return false;
//	}
//
//	if(pRoleSummry->GetLevel() < GetGlobalConfig().InvFightRoleLevel)//等级不实时
//	{
//		return false;
//	}
//
//	return true;
//}

bool InvFightRequest::SceneCondition(CRole* pRole)
{
	if(NULL == pRole)
	{
		return false;
	}

	if(pRole->IsInBattle())//非主城和公会
	{
		return false;	
	}

	return true;
}
