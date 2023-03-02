#include "pch.h"
#include "define/itemdef.h"
#include "custombattlemgr.h"
#include "role/role.h"
#include "network/worldlink.h"
#include "config.h"
#include "custombattle/custombattle.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlegenerator.h"
#include "custombattle/rpcm2k_custombattleworldop.h"
#include "custombattle/rpcm2g_custombattlegsop.h"
#include "custombattle/custombattlerolemgr.h"
#include "role/rolesummarymgr.h"

KKSG::ErrorCode CustomBattleMgr::TransReqToWorld(const CustomBattleReq& req)
{
	KKSG::ErrorCode errorcode = KKSG::ERR_FAILED;
	switch(req.op)
	{
	case KKSG::CustomBattle_Query:
	case KKSG::CustomBattle_QueryOne:
	case KKSG::CustomBattle_QuerySelf:
	case KKSG::CustomBattle_QueryRandom:
	case KKSG::CustomBattle_Search:

	case KKSG::CustomBattle_Modify:
	case KKSG::CustomBattle_StartNow:
	case KKSG::CustomBattle_DoCreate:

	case KKSG::CustomBattle_Join:
	case KKSG::CustomBattle_DoJoin:
	case KKSG::CustomBattle_UnJoin:

	case KKSG::CustomBattle_Match:
	case KKSG::CustomBattle_UnMatch:

	case KKSG::CustomBattle_ClearCD:
	case KKSG::CustomBattle_DoClearCD:
	case KKSG::CustomBattle_Reward:
		{
	        RpcM2K_CustomBattleWorldOp* worldop = RpcM2K_CustomBattleWorldOp::CreateRpc();

			worldop->m_oArg.set_op(req.op);
			worldop->m_oArg.set_delayid(req.delayid);
			worldop->m_oArg.set_serverid(MSConfig::Instance()->GetServerID());
			worldop->m_oArg.set_uid(req.uid);
			worldop->m_oArg.set_password(req.password);
			worldop->m_oArg.set_battlename(req.battlename);
			worldop->m_oArg.set_token(req.token);
			worldop->m_oArg.set_roleid(req.roleid);
			worldop->m_oArg.set_querycross(req.iscross);
			*worldop->m_oArg.mutable_config() = req.conf;
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(req.roleid);
			if (NULL != summary)
			{
				worldop->m_oArg.set_rolename(summary->GetName());
				worldop->m_oArg.set_level(summary->GetLevel());
				worldop->m_oArg.set_isgmjoin(IsGMJoin(summary->GetAccount()));
			}

            if (KKSG::CustomBattle_Match == req.op)
            {
                if (NULL != req.matchinfo)
                {
                    *worldop->m_oArg.mutable_matchinfo() = *req.matchinfo;
                }
                else
                {
                    SSWarn<<"matchinfo is NULL, roleid:"<<req.roleid<<" battleuid:"<<req.uid<<END;
                }
            }

			if (!WorldLink::Instance()->SendTo(*worldop))
			{
				return KKSG::ERR_CUSTOMBATTLE_WORLDNOTOPEN;
			}
			errorcode = KKSG::ERR_SUCCESS;
			break;
		}
	default:
		break;
	}

	return errorcode;
}


bool CustomBattleMgr::HandleWorldReply(const CustomBattleReq& req, KKSG::ErrorCode errorcode, const KKSG::CustomBattleClientInfo& clientinfo)
{
	switch(req.op)
	{
	case KKSG::CustomBattle_Query:
	case KKSG::CustomBattle_QueryOne:
	case KKSG::CustomBattle_QuerySelf:
	case KKSG::CustomBattle_QueryRandom:
		{
			m_query.HandleWorldReply(req, errorcode, clientinfo);
			break;
		}
	case KKSG::CustomBattle_Search:
		{
			m_search.HandleWorldReply(req, errorcode, clientinfo);
			break;
		}
	case KKSG::CustomBattle_Modify:
	case KKSG::CustomBattle_StartNow:
		{
			m_modify.HandleWorldReply(req, errorcode, clientinfo);
			break;
		}
	case KKSG::CustomBattle_DoCreate:
		{
			m_createworld.HandleWorldReply(req, errorcode, clientinfo);
			break;
		}
	case KKSG::CustomBattle_Join:
	case KKSG::CustomBattle_DoJoin:
		{
			if (CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				m_joinsystem.HandleWorldReply(req, errorcode, clientinfo);
			}
			else
			{
				m_joinworld.HandleWorldReply(req, errorcode, clientinfo);
			}
			break;
		}
	case KKSG::CustomBattle_UnJoin:
		{
			if (CCustomBattleConfig::Instance()->IsSystem(req.uid))
			{
				m_unjoinsystem.HandleWorldReply(req, errorcode, clientinfo);
			}
			else
			{
				m_unjoinworld.HandleWorldReply(req, errorcode, clientinfo);
			}
			break;
		}
	case KKSG::CustomBattle_Match:
		{
			m_match.HandleWorldReply(req, errorcode, clientinfo);
			break;
		}
	case KKSG::CustomBattle_Reward:
	case KKSG::CustomBattle_ClearCD:
	case KKSG::CustomBattle_DoClearCD:
		{
			m_rewardworld.HandleWorldReply(req, errorcode, clientinfo);
			break;
		}
	default:
		{
			SSWarn<<"invalid op:"<<req.op<<" roleid:"<<req.roleid<<END;
			return false;
			break;
		}
	}
	return true;
}
