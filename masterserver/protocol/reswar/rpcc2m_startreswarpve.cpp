#include "pch.h"
#include "reswar/rpcc2m_startreswarpve.h"
#include "reswar/guildbuffmgr.h"
#include "reswar/reswarmgr.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "timeutil.h"

// generate by ProtoGen at date: 2016/11/3 15:56:53

RPC_SERVER_IMPLEMETION(RpcC2M_StartResWarPVE, ResWarPVEArg, ResWarPVERes)

void RpcC2M_StartResWarPVE::OnCall(const ResWarPVEArg &roArg, ResWarPVERes &roRes)
{
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (!pRole) 
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Team *pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
	KKSG::ErrorCode error = ResWarMgr::Instance()->Check(pTeam);
	if (error != KKSG::ERR_SUCCESS) 
	{
		roRes.set_error(error);
		return;
	}
	if(pTeam->GetLeader()!= pRole->GetID())
	{
		roRes.set_error(ERR_RESWAR_LEADER);
		return;
	}

	UINT64 nGuildID = ResWarMgr::Instance()->GetGuildID(pTeam);
	UINT32 nTeamID  = pTeam->GetID();
	UINT32 nRes     = roArg.mine();
	if (nRes == 0)
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}
	GuildTeam*pGuildTeam = ResWarMgr::Instance()->GetGuildTeam(nGuildID);
	if (!pGuildTeam) 
	{
		roRes.set_error(KKSG::ERR_RESWAR_GROUP);
		return;
	}
	GuildFightTeam *pFightTeam = ResWarMgr::Instance()->GetGuildFightTeam(nGuildID, nTeamID);
	if(!pFightTeam)
	{
		roRes.set_error(KKSG::ERR_RESWAR_TEAM);
		return;
	}
	if (ResWarMgr::Instance()->GetTeamOnline(pFightTeam)<3)
	{
		roRes.set_error(ERR_RESWAR_LACKPLAYER);
		return;
	}
	UINT32 nSize = pFightTeam->vecMineral.size();
	if (pFightTeam->vecMineral.size()&&nRes<=pFightTeam->vecMineral.size()&&nRes<=pFightTeam->vecBuff.size())
	{
		UINT32 nMineID = pFightTeam->vecMineral[nRes-1];
		UINT32 nBuffID = pFightTeam->vecBuff[nRes-1];
		ResWarMgr::Instance()->StartBattlePVE(pGuildTeam,pFightTeam,nMineID,nBuffID);
	}
}

void RpcC2M_StartResWarPVE::OnDelayReplyRpc(const ResWarPVEArg &roArg, ResWarPVERes &roRes, const CUserData &roUserData)
{
}
