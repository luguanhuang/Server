#include "pch.h"
#include "team/rpcm2g_teamrequestm2g.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/teamrequest.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/team.h"
#include "table/expeditionconfigmgr.h"
#include "gamelogic/levelsealMgr.h"
#include "idip/idiprecord.h"
#include "gamelogic/teamrecord.h"
#include "define/teamdef.h"
#include "gamelogic/rolepartner.h"

// generate by ProtoGen at date: 2016/8/18 20:01:24

RPC_SERVER_IMPLEMETION(RpcM2G_TeamRequestM2G, TeamOPM2GArg, TeamOPM2GRes)

int JudgeLevelSealType(UINT32 expid)
{
	ExpeditionTable::RowData* pExpedition = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expid);
	if(NULL == pExpedition)
	{
		return KKSG::ERR_TEAM_EXPEDITIONID_NOT_EXIST;
	}
	if(0 != pExpedition->LevelSealType && (GODDESS_TEAM_SCENE == pExpedition->Type || ENDLESSABYSS_SCENE == pExpedition->Type))
	{
		stServerLevelSealInfo pLevelSealInfo = CLevelSealMgr::Instance()->GetLevelSealInfo();
		if(pExpedition->LevelSealType != pLevelSealInfo.m_uType)
		{
			return KKSG::ERR_TEAM_SEAL_TYPE;
		}
	}
	return KKSG::ERR_SUCCESS;
}

void RpcM2G_TeamRequestM2G::OnCall(const TeamOPM2GArg &roArg, TeamOPM2GRes &roRes)
{
	const KKSG::TeamOPArg& c2mArg = roArg.c2marg();
	KKSG::TeamOPRes& c2mRes = *roRes.mutable_c2mres();

	UINT64 roleID = roArg.roleid();
	Role *pRole = RoleManager::Instance()->FindByRoleID(roleID);
	if (pRole == NULL)//在其它GS
	{
		c2mRes.set_result(KKSG::ERR_FAILED);
		if(KKSG::TEAM_GET_FULL_DATA == c2mArg.request())
		{
			c2mRes.set_result(KKSG::ERR_SUCCESS);
		}
		return;
	}

	KKSG::ErrorCode ret = KKSG::ERR_SUCCESS;

	TeamCopy pTeam;
	if(roArg.has_dataall())
	{
		pTeam.LoadT(roArg.dataall());
	}

	TeamRequest Tq(pRole, &pTeam);

	switch (c2mArg.request())//里面不要出现 return
	{
	case KKSG::TEAM_CREATE: 
		{
			ret = (KKSG::ErrorCode)JudgeLevelSealType(c2mArg.expid()); 
			if(KKSG::ERR_SUCCESS != ret)
			{
				break;
			}
			ret = (KKSG::ErrorCode)Tq.CreateExpenditionTeam(c2mArg.expid());
			if(KKSG::ERR_SUCCESS != ret)
			{
				break;
			}

			break;
		}
	case KKSG::TEAM_JOIN:   
		{
			ret = (KKSG::ErrorCode)Tq.JoinTeam();
			if(KKSG::ERR_SUCCESS != ret)
			{
				break;
			}
			break;
		}
	case KKSG::TEAM_QUERYCOUNT:
		{
			CTeamRecord* pRecord = pRole->Get<CTeamRecord>();
			
			Tq.GetTeamTypeCount(c2mRes);	
			c2mRes.set_goddessgetrewardscount(pRecord->GetLeftGoddessRewardToday()); // 剩余可以领取女神奖励的次数
			c2mRes.set_wnrewardleftcount(pRecord->GetWNRewardLeftCount());
			c2mRes.set_wnrewardmaxcount(pRecord->GetWNRewardMaxCount());
		}
		break;
	case KKSG::TEAM_START_BATTLE:
		ret = KKSG::ErrorCode(Tq.StartBattleBeginVote(c2mRes));
		break;
	case KKSG::TEAM_CHANGE_EPXTEAMID:
		{
			ret = (KKSG::ErrorCode)JudgeLevelSealType(c2mArg.expid()); 
			if(KKSG::ERR_SUCCESS != ret)
			{
				break;
			}
			ret = (KKSG::ErrorCode)Tq.ChangeExpID(c2mArg.expid(), c2mRes, *roRes.mutable_teamnothelper()); 
		}
		break;
	case KKSG::TEAM_INVITE:
		ret = (KKSG::ErrorCode)Tq.Invite(c2mArg.roleid()); 
		break;
	case KKSG::TEAM_START_MATCH:
	case KKSG::TEAM_DOWN_MATCH:
		{
			ret = (KKSG::ErrorCode)JudgeLevelSealType(c2mArg.expid()); 
			if(KKSG::ERR_SUCCESS != ret)
			{
				break;
			}
			ret = (KKSG::ErrorCode)Tq.StartMatch(c2mArg.expid());
		}
		break;	
	case KKSG::TEAM_COSTTYPE:
		{
			if (c2mArg.has_extrainfo())
			{
				ret = (KKSG::ErrorCode)Tq.HandleTeamCost(c2mArg.extrainfo().costindex());
			}
			else
			{
				ret = KKSG::ERR_FAILED;
			}
		}
		break;
	case KKSG::TEAM_START_BATTLE_AGREE:
		{
			if (pTeam.GetConf() && pTeam.GetConf()->Type == PARTNER_TEAM_SCENE)
			{
				RolePartner* p = pRole->Get<RolePartner>();	
				ret = p->CheckCanMakePartner();
			}
			break;
		}
	case KKSG::TEAM_BATTLE_CONTINUE:
		{
			ret = (KKSG::ErrorCode)Tq.BattleContinue(roRes);	
		}
		break;
	case KKSG::TEAM_MEMBER_TYPE:
		{
			KKSG::TeamMemberType memType = c2mArg.membertype();
			switch(memType)
			{
			case TMT_HELPER:
				{
					ret = (KKSG::ErrorCode)Tq.SwitchHelper(pTeam.GetExpID());
				}
				break;
			case TMT_USETICKET:
				{
					ret = (KKSG::ErrorCode)Tq.SwitchUseTicket();
				}
				break;
			case TMT_NORMAL:
				{
					;//need no handle
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		{
			SSInfo<<"gs can not handler this type:"<<c2mArg.request()<<END;
		}
		break;
	}

	if(KKSG::ERR_TEAM_IDIP == ret)//被禁止提示
	{
		pRole->Get<CIdipRecord>()->CheckIdipAndNotify(KKSG::PUNISH_USER_DAILY_PLAY ,true);	
	}

	c2mRes.set_result(ret);
}

void RpcM2G_TeamRequestM2G::OnDelayReplyRpc(const TeamOPM2GArg &roArg, TeamOPM2GRes &roRes, const CUserData &roUserData)
{
}
