#include "pch.h"
#include "team/rpck2m_teamstartbattlew2m.h"
#include "config.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "scene/sceneswitch.h"
#include "team/rpcm2g_teamstartbattlem2gcross.h"
#include "team/rpcm2g_teamstartbattlem2g.h"
#include "team/rpcm2g_teamtranms2gs.h"
#include "network/gslink.h"
#include "define/teamdef.h"
#include "team/teamstartbattle.h"

// generate by ProtoGen at date: 2017/6/27 10:26:32

RPC_SERVER_IMPLEMETION(RpcK2M_TeamStartBattleW2M, TeamStartBattleArg, TeamStartBattleRes)

void RpcK2M_TeamStartBattleW2M::OnCall(const TeamStartBattleArg &roArg, TeamStartBattleRes &roRes)
{
	if(!roArg.teamdata().has_csextra())
	{
		RpcM2G_TeamTranMs2Gs* rpc = RpcM2G_TeamTranMs2Gs::CreateRpc();
		rpc->m_oArg.set_type(TEAM_TRANS_START_BATTLE);
		*rpc->m_oArg.mutable_startbattlearg() = roArg;
		//rpc->m_oArg.set_delayid(DelayRpc());
		auto& teamData = *rpc->m_oArg.mutable_startbattlearg()->mutable_teamdata();
		teamData.clear_rolebufflist();
		std::vector<CRole*> roles;
		for(int i = 0; i < teamData.teamsyndata().members_size(); ++i)
		{
			UINT64 roleID = teamData.teamsyndata().members(i).roleid();
			UINT32 serverID = teamData.teamsyndata().members(i).serverid();
			if(MSConfig::Instance()->GetServerID() != serverID)
			{
				continue;
			}
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
			if(pRole)
			{
				roles.push_back(pRole);
			}
			else
			{
				SSWarn << " find role failed roleid = " << roleID << END;
			}
		}
		TeamStartBattle::FillBuffs(roles, teamData);
		GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
	}
	else
	{
		const auto& extra = roArg.teamdata().csextra();
		for(int i = 0; i < extra.members_size(); ++i)
		{
			UINT64 roleID = extra.members(i).roleid();
			UINT32 serverID = extra.members(i).serverid();
			if(MSConfig::Instance()->GetServerID() != serverID)
			{
				continue;
			}
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
			if(NULL == pRole)
			{
				SSWarn << " find role failed roleid = " << roleID << END;
				continue;
			}
			if(extra.type() == TGOTONST_NEXT)
			{
				KKSG::SceneSwitchData tmpData;
				CSceneSwitch::EnterScene(pRole, roArg.sceneid(), tmpData, roArg.gsline(), true, roArg.mapid());
			}
			else if(extra.type() == TGOTONST_RETURN)
			{
				//CSceneSwitch::EnterMainHall(pRole); 原来不支持。。。
			}
		}
	}
}

void RpcK2M_TeamStartBattleW2M::OnDelayReplyRpc(const TeamStartBattleArg &roArg, TeamStartBattleRes &roRes, const CUserData &roUserData)
{
}
