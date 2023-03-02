#include "pch.h"
#include "invfightcrossrequest.h"
#include "role/role.h"
#include "role/rolemanager.h"
//#include "invfightmgr.h"
#include "invfightcrossmgr.h"
#include "table/globalconfig.h"
#include "invfight/ptcm2c_invfightnotify.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "invfight/rpcc2m_invfightreqall.h"
#include "invfight/invfightrequest.h"
#include "platforminfo/platformfriendmgr.h"
#include "config.h"
#include "scene/rpcm2k_createcrossbattlescene.h"
#include "network/worldlink.h"
#include "loghelper/tlogr.h"

InvFightCrossRequest::InvFightCrossRequest(CRole* role)
{
	m_pRole = role;
}

InvFightCrossRequest::~InvFightCrossRequest()
{

}

int InvFightCrossRequest::CheckPreInv(const std::string& toAccount)
{
	if(!SceneCondition(m_pRole))//先判断场景
	{
		return KKSG::ERR_INVFIGHT_ME_SCENE;
	}

	if (!PlatFriendCondition(toAccount))
	{
		return KKSG::ERR_FAILED;
	}

	if(InvFightCrossMgr::Instance()->InvFromCount(m_pRole->GetID()) >= GetGlobalConfig().InvFightFromCount)
	{
		return KKSG::ERR_INVFIGHT_INV_COUNT_MAX;
	}

	if(InvFightCrossMgr::Instance()->HaveInvTo(m_pRole->GetID(), toAccount))
	{
		return KKSG::ERR_INVFIGHT_INV_REPEAT;
	}

	if(InvFightCrossMgr::Instance()->InvFromCount(m_pRole->GetID()) >= GetGlobalConfig().InvFightFromCount)
	{
		return KKSG::ERR_INVFIGHT_INV_COUNT_MAX;
	}

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::CheckPreBeInv(UINT64 fromID)
{
	if(!SceneCondition(m_pRole))//先判断场景
	{
		return KKSG::ERR_INVFIGHT_ME_SCENE;
	}

	if(InvFightCrossMgr::Instance()->HaveInvTo(fromID, m_pRole->GetAccount()))
	{
		return KKSG::ERR_INVFIGHT_INV_REPEAT;
	}

	UINT32 CurInvFightCrossToCount = InvFightCrossMgr::Instance()->InvToCount(m_pRole->GetAccount());
	UINT32 CurInvFightsToCount = InvFightMgr::Instance()->InvToCount(m_pRole->GetID());
	if(CurInvFightCrossToCount + CurInvFightsToCount >= InvFightToCountMax)
	{
		return KKSG::ERR_INVFIGHT_INV_TO_COUNT_MAX;
	}

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::InvFromOne(UINT64 invID, const std::string& toAccount/*, KKSG::InvFightM2GRes& m2gRes*/)
{
	if(!SceneCondition(m_pRole))//先判断场景
	{
		return KKSG::ERR_INVFIGHT_ME_SCENE;
	}

	if(InvFightCrossMgr::Instance()->InvFromCount(m_pRole->GetID()) >= GetGlobalConfig().InvFightFromCount)
	{
		return KKSG::ERR_INVFIGHT_INV_COUNT_MAX;
	}

	if(InvFightCrossMgr::Instance()->HaveInvTo(m_pRole->GetID(), toAccount))
	{
		return KKSG::ERR_INVFIGHT_INV_REPEAT;
	}

	if(InvFightCrossMgr::Instance()->InvFromCount(m_pRole->GetID()) >= GetGlobalConfig().InvFightFromCount)
	{
		return KKSG::ERR_INVFIGHT_INV_COUNT_MAX;
	}

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::InvToOne(UINT64 invID, UINT64 fromID/*, KKSG::InvFightM2GRes& m2gRes*/)
{
	if(!SceneCondition(m_pRole))//先判断场景
	{
		return KKSG::ERR_INVFIGHT_OTHER_SCENE;
	}

	if(InvFightCrossMgr::Instance()->HaveInvTo(fromID, m_pRole->GetAccount()))
	{
		return KKSG::ERR_INVFIGHT_INV_REPEAT;
	}


	UINT32 CurInvFightCrossToCount = InvFightCrossMgr::Instance()->InvToCount(m_pRole->GetAccount());
	UINT32 CurInvFightsToCount = InvFightMgr::Instance()->InvToCount(m_pRole->GetID());
	if(CurInvFightCrossToCount + CurInvFightsToCount >= InvFightToCountMax)
	{
		return KKSG::ERR_INVFIGHT_INV_TO_COUNT_MAX;
	}

	//notify
	PtcM2C_InvFightNotify ptc;
	ptc.m_Data.set_ntftype(KKSG::IFNT_INVITE_ME);
	ptc.m_Data.set_count(CurInvFightCrossToCount + CurInvFightsToCount + 1);
	m_pRole->Send(ptc);

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::RefushOne(UINT64 invID)
{
	auto pUniRec = InvFightCrossMgr::Instance()->GetInvUniRec(invID);
	if(NULL == pUniRec)//已过期
	{
		return KKSG::ERR_SUCCESS;
	}

	//if(pUniRec->InvToID() != m_pRole->GetID())
	if(strcmp(pUniRec->InvToAccount().c_str(), m_pRole->GetAccount().c_str()) != 0)
	{
		return KKSG::ERR_FAILED;
	}

	//UINT64 fromID = pUniRec->InvFromID();

	//InvFightCrossMgr::Instance()->Del(invID);

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::RefushOneFrom(UINT64 invID)
{
	auto pUniRec = InvFightCrossMgr::Instance()->GetInvUniRec(invID);
	if(NULL == pUniRec)//已过期
	{
		return KKSG::ERR_SUCCESS;
	}

	if(pUniRec->InvFromID() != m_pRole->GetID())
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 fromID = pUniRec->InvFromID();


	CRole* pFromRole = CRoleManager::Instance()->GetByRoleID(fromID);
	if(pFromRole && SceneCondition(pFromRole))
	{
		PtcM2C_InvFightNotify ptc;
		ptc.m_Data.set_ntftype(KKSG::IFNT_REFUSE_ME);
		PlatFriendList* list = CPlatformFriendMgr::Instance()->GetPlatFriendList(m_pRole);
		if (list)
		{
			auto info = list->GetPlatFriendUserInfo(pUniRec->InvToAccount());
			if (info)
			{
				ptc.m_Data.set_name(info->nickname);
			}
		}
		pFromRole->Send(ptc);
	}
	
	InvFightCrossMgr::Instance()->Del(invID);

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::IgnoreAll()
{
	InvFightCrossMgr::Instance()->DelToIDInv(m_pRole->GetAccount());

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::ReqList(KKSG::InvFightRes& roRes)
{
	InvFightCrossMgr::Instance()->FillInvToList(m_pRole->GetAccount(), roRes);

	if(roRes.roles_size() <= 0)
	{
		return KKSG::ERR_INVFIGHT_INV_TIME_OVER;
	}

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::AcceptPre(UINT64 invID)
{
	auto pUniRec = InvFightCrossMgr::Instance()->GetInvUniRec(invID);
	if(NULL == pUniRec)
	{
		return KKSG::ERR_INVFIGHT_INV_TIME_OVER;
	}
	//if (pUniRec->InvToID() != m_pRole->GetID())
	if(strcmp(pUniRec->InvToAccount().c_str(), m_pRole->GetAccount().c_str()) != 0)
	{
		return KKSG::ERR_FAILED;
	}
	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::AcceptOneFrom(UINT64 invID)
{
	if(!SceneCondition(m_pRole))
	{
		return KKSG::ERR_INVFIGHT_OTHER_SCENE;
	}

	auto pUniRec = InvFightCrossMgr::Instance()->GetInvUniRec(invID);
	if(NULL == pUniRec)
	{
		return KKSG::ERR_INVFIGHT_INV_TIME_OVER;
	}

	InvFightCrossMgr::Instance()->Del(invID);

	return KKSG::ERR_SUCCESS;
}

int InvFightCrossRequest::AcceptOneTo(UINT64 invID)
{
	if(!SceneCondition(m_pRole))
	{
		return KKSG::ERR_INVFIGHT_ME_SCENE;
	}

	InvFightCrossMgr::Instance()->Del(invID);

	return KKSG::ERR_SUCCESS;
}

bool InvFightCrossRequest::ProcessClientCall(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, InvFightCrossProcessDelayInfo& delayInfo)
{
	bool needDelay = false;
	int ret = int(KKSG::ERR_SUCCESS);
	bool gotocs = false;
	UINT64 fromRoleID = 0;
	static UINT32 s_newInvID = 0;

	switch(roArg.reqtype())
	{
	case IFRT_INV_ONE:
		{
			gotocs = true;
			if (!roArg.has_account())
			{
				ret = KKSG::ERR_FAILED;
				break;
			}

			ret = CheckPreInv(roArg.account());
			delayInfo.fromServer = MSConfig::Instance()->GetServerID();
			delayInfo.fromID = m_pRole->GetID();
			delayInfo.toAccount = roArg.account();
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pRole->GetID());
			if (summary)
			{
				KKSG::RoleSmallInfo* smallInfo = delayInfo.smallInfo.add_roles();
				summary->FillSmallInfo(*smallInfo);
			}

			
			if (ret == KKSG::ERR_SUCCESS)
			{
				++s_newInvID;
				if (s_newInvID == 0)
				{
					s_newInvID = 1;
				}
				UINT64 newInvID = delayInfo.fromServer;
				newInvID <<= 32;
				delayInfo.invID = newInvID + s_newInvID;
			}
			
		}
		break;
	case IFRT_REFUSH_ONE:
		{
			ret = RefushOne(roArg.invid());

			auto pUniRec = InvFightCrossMgr::Instance()->GetInvUniRec(roArg.invid());
			if(NULL == pUniRec)//已过期
			{
				ret = KKSG::ERR_SUCCESS;
				break;
			}
			gotocs = true;

			delayInfo.FillFromInvFightCrossRequest(*pUniRec);

			//InvFightCrossMgr::Instance()->Del(roArg.invid());
		}
		break;
	case IFRT_ACCEPT_ONE:
		{
			ret = AcceptPre(roArg.invid());
			auto pUniRec = InvFightCrossMgr::Instance()->GetInvUniRec(roArg.invid());
			if(NULL == pUniRec)//已过期
			{
				ret = KKSG::ERR_INVFIGHT_INV_TIME_OVER;
				break;
			}
			gotocs = true;

			delayInfo.FillFromInvFightCrossRequest(*pUniRec);
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_pRole->GetID());
			if (summary)
			{
				summary->FillSmallInfo(*delayInfo.smallInfo.add_roles());
			}
		}
		break;
	default:
		break;
	}
	
	if(ERR_SUCCESS != ret || false == gotocs)
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

	return needDelay;
}

void InvFightCrossRequest::ProcessClientCallDelay(const KKSG::InvFightArg &roArg, KKSG::InvFightRes &roRes, const CUserData &roUserData)
{
	roRes.set_isplatfriendonline(true);
	if (roArg.reqtype() == IFRT_INV_ONE && roUserData.m_dwUserData == KKSG::ERR_ROLE_NOT_ONLINE)
	{
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
		roRes.set_isplatfriendonline(false);
	}
	else if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		if (roArg.reqtype() == IFRT_REFUSH_ONE)
		{
			InvFightCrossMgr::Instance()->Del(roArg.invid());
			roRes.set_errorcode(KKSG::ERR_SUCCESS);
			return;
		}

		roRes.set_errorcode((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}

	InvFightCross2CSReqRes* poRes = (InvFightCross2CSReqRes*)(roUserData.m_pUserPtr);

	int ret = int(ERR_SUCCESS);
	switch(roArg.reqtype())
	{
	case IFRT_INV_ONE:
		{
			//ret = CheckPreInv(roArg.account());
			ret = InvFromOne(poRes->invmsg().invid(), poRes->invmsg().toaccount());
			if (ret == KKSG::ERR_SUCCESS)
			{
				InvFightCrossMgr::Instance()->AddFromCrossFight(poRes->invmsg());
			}
		}
		break;
	case IFRT_ACCEPT_ONE:
		{
			ret = AcceptOneTo(roArg.invid());
			if (ret == KKSG::ERR_SUCCESS)
			{
				//request fight
				poRes->mutable_invmsg()->set_toid(m_pRole->GetID());
				ReqEnterBattle(poRes->invmsg());
			}
		}
		break;
	case IFRT_REFUSH_ONE:
		{
			InvFightCrossMgr::Instance()->Del(roArg.invid());
			roRes.set_errorcode(KKSG::ERR_SUCCESS);
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

bool InvFightCrossRequest::SceneCondition(CRole* pRole)
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

bool InvFightCrossRequest::PlatFriendCondition(const std::string& account)
{
	PlatFriendList* list = CPlatformFriendMgr::Instance()->GetPlatFriendList(m_pRole);
	if (!list)
	{
		return false;
	}

	if(list->GetPlatFriendUserInfo(account))
		return true;

	return false;
}

void InvFightCrossRequest::ReqEnterBattle(const KKSG::InvFightCrossMsg& msg)
{
	RpcM2K_CreateCrossBattleScene* rpc = RpcM2K_CreateCrossBattleScene::CreateRpc();
	rpc->m_oArg.set_mapid(GetGlobalConfig().InvFightMapID);

	KKSG::CreateCrossBattleSceneData* info = rpc->m_oArg.add_createinfos();
	info->set_roleid(msg.fromid());
	info->set_serverid(msg.fromsrverid());

	info = rpc->m_oArg.add_createinfos();
	info->set_roleid(msg.toid());
	info->set_serverid(msg.toserverid());
	
	rpc->m_oArg.mutable_smallinfo()->CopyFrom(msg.smallinfo());

	WorldLink::Instance()->SendTo(*rpc);

	TInvFightFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_strFromOpenId = msg.fromaccount();
	oLog.m_FromId = msg.fromid();
	oLog.m_ToId = msg.toid();
	oLog.m_strToOpenId = msg.toaccount();
	oLog.SetData(msg.smallinfo());
	oLog.Do();
}

void InvFightCrossProcessDelayInfo::FillFromInvFightCrossRequest(InvCrossRec& rec)
{
	fromServer = rec.InvFromServer();
	toServer = rec.InvToServer();
	fromID = rec.InvFromID();
	toID = rec.InvToID();
	fromAccount = rec.InvFromAccount();
	toAccount = rec.InvToAccount();
	invID = rec.InvID();
}
