#include "pch.h"
#include "time.h"
#include "math.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "friend/friend.h"
#include "friend/friendop.h"
#include "friend/rpcc2m_randomfriendwaitlistnew.h"

// generate by ProtoGen at date: 2016/7/28 23:50:47
#define MAXSELECT 4

#define FILTER_LEVEL 5
#define FILTER_PPT 0.2f


UINT32 FilterLevelAndPPT(CRoleSummary* me, CRoleSummary* other)
{
	if (me->GetID() == other->GetID())
	{
		return 2;
	}
	if (abs((int)me->GetLevel() - (int)other->GetLevel()) <= FILTER_LEVEL)
	{
		return 0;
	}
	int pptme = (int)me->GetPowerPoint();
	int pptother = (int)other->GetPowerPoint();
	if (pptme == 0)
	{
		return 2;			
	}
	float scale = (float)abs(pptme - pptother)/(float)pptme;
	if (scale <= FILTER_PPT)
	{
		return 0;
	}
	return 1;
}

bool SelectRole(time_t nowtime, CFriendRecord* record, FriendOp& op, std::set<UINT64>& selectids, std::vector<UINT64>& roleids)
{
	for (auto i = roleids.begin(); i != roleids.end(); ++i)
	{
		if (selectids.size() >= MAXSELECT)
		{
			return true;
		}
		UINT64 id = *i;
		if (record->IsRandomValid(nowtime, id) && !op.IsFriend(id) && !op.IsInBlackList(id))
		{
			selectids.insert(id);
		}
	}
	return false;
}

void PackRes(time_t nowtime, CFriendRecord* record, std::set<UINT64>& selectids, RandomFriendWaitListRes &roRes)
{
	for (auto j = selectids.begin(); j != selectids.end(); ++j)
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(*j);
		if (NULL != summary)
		{
			roRes.add_roleid(summary->GetID());	
			roRes.add_profession(summary->GetProfession());	
			roRes.add_level(summary->GetLevel());	
			roRes.add_name(summary->GetName());	
			roRes.add_powerpoint(summary->GetPowerPoint());	
			roRes.add_viplevel(summary->GetVipLevel());
			roRes.add_nickid(summary->GetData().nickid());
			roRes.add_titleid(summary->GetTitleID());
			roRes.add_lastlogin(summary->GetLastLogout());

			UINT64 guildid = 0;
			std::string guildname;
			Guild* guild = CGuildMgr::Instance()->GetGuild(summary->GetID());
			if (NULL != guild)
			{
				guildid = guild->GetID();
				guildname = guild->GetName();
			}
			roRes.add_guildid(guildid);
			roRes.add_guildname(guildname);

			record->AddRandom(nowtime, *j);
		}
	}
	roRes.set_errorcode(ERR_SUCCESS);
}

RPC_SERVER_IMPLEMETION(RpcC2M_RandomFriendWaitListNew, RandomFriendWaitListArg, RandomFriendWaitListRes)

void RpcC2M_RandomFriendWaitListNew::OnCall(const RandomFriendWaitListArg &roArg, RandomFriendWaitListRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
	if (NULL == summary)
	{
		return;
	}
	FriendOp op(role->GetID());
	CFriendRecord* record = op.GetRecord();
	if (!record->UpdateQueryTime())
	{
		roRes.set_errorcode(ERR_RANDOMFRIEND_CD);
		return;
	}
	// condition
	if (op.IsFriendMax())
	{
		roRes.set_errorcode(ERR_FRIEND_MAX);
		return;
	}

	time_t nowtime = time(NULL);
	record->UpdateRandom(nowtime);

	// filter
	std::set<UINT64> selectids;
	// guild
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (NULL != guild)
	{
		for (auto i = guild->GetGuildMember().begin(); i != guild->GetGuildMember().end(); ++i)
		{
			UINT64 id = i->first;
			if (role->GetID() == id)
			{
				continue;
			}
			if (selectids.size() >= MAXSELECT)
			{
				PackRes(nowtime, record, selectids, roRes);
				return;
			}
			if (record->IsRandomValid(nowtime, id) && !op.IsFriend(id) && !op.IsInBlackList(id))
			{
				selectids.insert(id);
			}
		}
	}
	// online 
	std::vector<UINT64> onlineroles;
	std::vector<UINT64> onlineexroles;
	CRoleManager::Instance()->GetFilterRole(summary, FilterLevelAndPPT, onlineroles, onlineexroles);
	if (SelectRole(nowtime, record, op, selectids, onlineroles))
	{
		PackRes(nowtime, record, selectids, roRes);
		return;
	}
	if (SelectRole(nowtime, record, op, selectids, onlineexroles))
	{
		PackRes(nowtime, record, selectids, roRes);
		return;
	}
	// all
	//std::vector<UINT64> allroles;
	//std::vector<UINT64> allexroles;
	//RoleSummaryMgr::Instance()->GetFilterRole(role, FilterLevelAndPPT, allroles, allexroles);
	//if (SelectRole(nowtime, record, op, selectids, allroles))
	//{
	//	PackRes(nowtime, record, selectids, roRes);
	//	return;
	//}
	//if (SelectRole(nowtime, record, op, selectids, allexroles))
	//{
	//	PackRes(nowtime, record, selectids, roRes);
	//	return;
	//}
	// last
	for (auto i = CRoleSummaryMgr::Instance()->Begin(); i != CRoleSummaryMgr::Instance()->End(); ++i)
	{
		UINT64 id = i->second->GetID();
		if (role->GetID() == id)
		{
			continue;
		}
		if (selectids.size() >= MAXSELECT)
		{
			PackRes(nowtime, record, selectids, roRes);
			return;
		}
		if (record->IsRandomValid(nowtime, id) && !op.IsFriend(id) && !op.IsInBlackList(id))
		{
			selectids.insert(id);
		}
	}
}

void RpcC2M_RandomFriendWaitListNew::OnDelayReplyRpc(const RandomFriendWaitListArg &roArg, RandomFriendWaitListRes &roRes, const CUserData &roUserData)
{
}
