#include "pch.h"
#include "role/rpcc2g_personalcareer.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/payconfig.h"
#include "util/gametime.h"
#include "role/rpcg2m_personalcareertotalinfo.h"
#include "network/mslink.h"
#include "role/rpcg2m_personalcareertotalinfo.h"
#include "table/globalconfig.h"
#include "gamelogic/PersonalCareerQuest.h"
#include "gamelogic/stagemgr.h"
// generate by ProtoGen at date: 2017/2/4 16:33:57

RPC_SERVER_IMPLEMETION(RpcC2G_PersonalCareer, PersonalCareerArg, PersonalCareerRes)

	void RpcC2G_PersonalCareer::OnCall(const PersonalCareerArg &roArg, PersonalCareerRes &roRes)
{
	Role * role = NULL;
	if (roArg.role_id() > 0)
	{
		role = RoleManager::Instance()->FindByRoleID(roArg.role_id());
		if (NULL == role)
		{
			roRes.set_result(KKSG::ERR_QUESTCAREER_NOT_ONLINE);
			return;	
		}
	}
	else
	{
		role = RoleManager::Instance()->FindBySession(m_sessionID);
	}

	if (role == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	RpcG2M_PersonalCareerTotalInfo* msg = RpcG2M_PersonalCareerTotalInfo::CreateRpc();
	msg->m_oArg.set_rpc_id(DelayRpc());
	msg->m_oArg.set_role_id(role->GetID());
	msg->m_oArg.set_quest_type(roArg.quest_type());
	MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), *msg);
}

void RpcC2G_PersonalCareer::OnDelayReplyRpc(const PersonalCareerArg &roArg, PersonalCareerRes &roRes, const CUserData &roUserData)
{
	LogDebug("type is %d\n",roArg.quest_type());

	Role * role = NULL;
	if (roArg.role_id() > 0)
	{
		role = RoleManager::Instance()->FindByRoleID(roArg.role_id());
		if (NULL == role)
		{
			roRes.set_result(KKSG::ERR_QUESTCAREER_NOT_ONLINE);
			return;	
		}	
	}
	else
	{
		role = RoleManager::Instance()->FindBySession(m_sessionID);
	}

	if (role == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	PersonalCareerTotalInfoRes* res = (PersonalCareerTotalInfoRes*)roUserData.m_pUserPtr;
	if (NULL == res)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}

	switch (roArg.quest_type())
	{
	case KKSG::PCRT_HOME_PAGE:
		{
			PersonalHomePage* home_page = roRes.mutable_home_page();
			PersonalCareerQuest::Instance()->GetHomePage(*res,*role,*home_page);
			roRes.set_result(KKSG::ERR_SUCCESS);
		}
		break;
	case KKSG::PCRT_PVP_PKINFO:
		{
			bool open = false;
			roRes.set_result(KKSG::ERR_SUCCESS);

			auto it = GetGlobalConfig().PersonalCareerPVP.begin();
			auto it_end = GetGlobalConfig().PersonalCareerPVP.end();
			for (; it != it_end;++it)
			{
				if (role->Get<CRoleSystem>()->IsSystemOpened(*it))
				{
					roRes.add_system_status(true);
					open = true;
				}
				else
				{
					roRes.add_system_status(false);
				}
			}

			if (!open)
			{
				roRes.set_result(KKSG::ERR_CAREER_PVP_NOTOPEN);
				return ;
			}

			KKSG::PVPInformation* pvp = roRes.mutable_pvp_info();
			PersonalCareerQuest::Instance()->GetPVPInformation(*res,*role,*pvp);
		}
		break;
	case KKSG::PCRT_TROPHY:
		{
			roRes.set_result(KKSG::ERR_SUCCESS);
			role->Get<StageMgr>()->GetTrophyData(roRes);
		}
		break;
	default:
		break;
	}		
}
