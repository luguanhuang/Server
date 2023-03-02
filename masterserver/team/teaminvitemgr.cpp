#include "pch.h"
#include "teaminvitemgr.h"
#include "team.h"
#include "teamrequest.h"
#include "table/expeditionconfigmgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "teammgr.h"
#include "table/globalconfig.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/ptcm2c_teaminvitem2cnotify.h"
#include "common/ptcm2c_mserrornotify.h"
#include "team/ptcm2c_inviterefusem2cntf.h"
#include "role/rolesummarymgr.h"
#include "team/ptcm2c_invunfstatem2cntf.h"
#include "team/teamcommon.h"
#include "teamtranshandler.h"

INSTANCE_SINGLETON(TeamInviteMgr)

TeamInviteMgr::TeamInviteMgr()
{

}

TeamInviteMgr::~TeamInviteMgr()
{

}

bool TeamInviteMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);
	return true;
}

void TeamInviteMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_handler);
}

void TeamInviteMgr::MakeInvite(KKSG::TeamInvite& data)
{
	UINT64 invFromID = data.invfromroleid();
	UINT64 invToID = data.invtoroleid();

	if(0 == invFromID || 0 == invToID)
	{
		SSError << " invite fromid = " << invFromID << " toid = " << invToID << END;
		return;
	}

	if(IsInRefuseForNow(invToID))//暂不接受
	{
		return;
	}
	if(IsInInviting(data.invfromroleid(), invToID))//忽略重复
	{
		return;
	}

	if(m_invs.size() >= 100000)//过多 则系统忽略掉
	{
		SSWarn << " inv size = " << m_invs.size() << END;
		return;
	}

	UINT32 inviteID = NewInviteID(); 
	data.set_inviteid(inviteID);
	TeamInviteRec rec(data);

	auto ret = m_invs.insert(std::make_pair(rec.GetInviteID(), rec));
	if(!ret.second)
	{
		SSError << " add invite failed inviteid = " << rec.GetInviteID() << " from = " << rec.GetInvFromID() << " to = " << rec.GetInvToID() << END;
		return;
	}

	CRole* pToRole = CRoleManager::Instance()->GetByRoleID(invToID);
	if(pToRole)
	{
		PtcM2C_TeamInviteM2CNotify ptc;
		FillInviteData(ptc.m_Data, rec);
		pToRole->Send(ptc);
	}
}

void TeamInviteMgr::OnAck(CRole * pRole, UINT32 inviteID, bool accept)
{
	if(NULL == pRole)
	{
		return;
	}

	auto it = m_invs.find(inviteID);
	if(it == m_invs.end())
	{
		pRole->ErrorCodeNtf(KKSG::ERR_TEAM_INV_TIMEOUT, true);
		return;
	}

	TeamInviteRec& rec = it->second;
	if (rec.GetInvToID() != pRole->GetID())//异常
	{
		SSWarn << " invtoid = " << rec.GetInvToID() << " role = " << pRole->GetID() << END;
		return;
	}

	if(HTeam::IsTsTeam(rec.GetTeamID()))//是跨服组队
	{
		TeamTransHandler trans;
		trans.HandleRoleAckToTs(pRole, rec, accept);
	}
	else
	{
		if(accept)
		{
			TeamRequest request(pRole);
			int ret = request.JoinTeam(rec.GetTeamID(), TEAM_OPT_NORMAL);
			if (ret != KKSG::ERR_SUCCESS)
			{
				pRole->ErrorCodeNtf(ret, true);
			}
		}
		else
		{
			RefuseNtf(rec.GetInvFromID(), pRole->GetName());
		}
	}

	m_invs.erase(inviteID);//删除(如果流程正确 则必须执行到这里)
}

bool TeamInviteMgr::IsInInviting(UINT64 fromID, UINT64 toID)
{
	for(auto it = m_invs.begin(); it != m_invs.end(); ++it)
	{
		auto& rec = it->second;
		if(rec.GetInvFromID() == fromID && rec.GetInvToID() == toID)
		{
			return true;
		}
	}
	return false;
}

void TeamInviteMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 now = UINT32(time(NULL));
	for(auto it = m_invs.begin(); it != m_invs.end();)
	{
		auto& rec = it->second;
		if(rec.GetPassTime(now) > GetGlobalConfig().TeamInvUnfConfirmTime)
		{
			DelInvNtf(rec.GetInviteID(), rec.GetInvToID());
			m_invs.erase(it++);			
		}
		else
		{
			++it;
		}
	}

	for(auto it = m_refuseForNow.begin(); it != m_refuseForNow.end();)
	{
		if(it->second + GetGlobalConfig().TeamUnfRefuseForNowT <= now)
		{
			m_refuseForNow.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void TeamInviteMgr::TestPrint()
{
	UINT32 now = UINT32(time(NULL));
	for(auto it = m_invs.begin(); it != m_invs.end(); ++it)
	{
		auto& rec = it->second;
		SSDebug << " invid = " << rec.GetInviteID() << " from = " << rec.GetInvFromID() << " to = " << rec.GetInvToID() << " passt = " << rec.GetPassTime(now) << END;
	}
}

void TeamInviteMgr::AddUnfRefuseForNow(UINT64 roleID)
{
	m_refuseForNow[roleID] = time(NULL);
}

bool TeamInviteMgr::IsInRefuseForNow(UINT64 roleID)
{
	if(m_refuseForNow.find(roleID) == m_refuseForNow.end())
	{
		return false;
	}
	return true;
}

void TeamInviteMgr::DelInvToMe(UINT64 invToID)
{
	for(auto it = m_invs.begin(); it != m_invs.end(); )
	{
		if((it->second).GetInvToID() == invToID)
		{
			m_invs.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void TeamInviteMgr::FillAllInvToMe(UINT64 invToID, KKSG::InvHistoryRes& roRes)
{
	for(auto it = m_invs.begin(); it != m_invs.end(); ++it)
	{
		auto& rec = it->second;
		if(rec.GetInvToID() != invToID)
		{
			continue;
		}
		FillInviteData(*roRes.add_invunfh(), rec);
		if(roRes.invunfh_size() > 100)//防止过多
		{
			break;
		}
	}
}

//跨服队伍需要处理 *****
void TeamInviteMgr::DelInvByTeam(int teamID)
{
	for(auto it = m_invs.begin(); it != m_invs.end();)
	{
		auto& rec = (it->second);
		if(rec.GetTeamID() == teamID)
		{
			DelInvNtf(rec.GetInviteID(), rec.GetInvToID());
			m_invs.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void TeamInviteMgr::FillInviteData(KKSG::TeamInvite& data, TeamInviteRec& rec)
{
	data = rec.Data();
	UINT32 now = UINT32(time(NULL));
	UINT32 passTime = rec.GetPassTime(now); 
	data.set_invtime(passTime);
}

void TeamInviteMgr::DelInvNtf(int inviteID, UINT64 invToID)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(invToID);
	if(pRole)
	{
		PtcM2C_InvUnfStateM2CNtf ntf;
		ntf.m_Data.set_inviteid(inviteID);
		ntf.m_Data.set_isdeleted(true);
		pRole->Send(ntf);
	}
}

UINT32 TeamInviteMgr::NewInviteID()
{
	static UINT32 InviteIndexID = 0;
	InviteIndexID ++;
	return InviteIndexID;
}

void TeamInviteMgr::RefuseNtf(UINT64 roleID, const std::string& name)
{
	CRole* invFromRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(invFromRole)//通知拒绝
	{
		PtcM2C_InviteRefuseM2CNtf refuseNtf;
		refuseNtf.m_Data.set_name(name);
		invFromRole->Send(refuseNtf);
	}
}

void TeamInviteMgr::ClearAll()
{
	m_invs.clear();
	m_refuseForNow.clear();
}

TeamInviteRec::TeamInviteRec(const KKSG::TeamInvite& data)
	:m_data(data)
{
}

TeamInviteRec::~TeamInviteRec()
{

}

UINT32 TeamInviteRec::GetPassTime(UINT32 now)
{
	return (now > m_data.invtime())?(now - m_data.invtime()):0;
}
