#include "pch.h"
#include "friend.h"
#include "time.h"
#include "frienddef.h"
#include "friendop.h"
#include "friendconfig.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "pb/project.pb.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "mentorship/mentormgr.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "platforminfo/platformfriendmgr.h"

UINT32 ErrorCodeTrans(UINT32 errorcode, bool isself)
{
	switch (errorcode)
	{
	case FriendMax: return isself ? KKSG::ERR_FRIEND_MAX : KKSG::ERR_FRIEND_MAXOTHER;
	case FriendRepeated: return KKSG::ERR_FRIEND_REPEATED;
	case FriendNotExist: return KKSG::ERR_FRIEND_NOTEXIST;
	case InSelfBlackList: return isself ? KKSG::ERR_BLACK_INSELF : KKSG::ERR_BLACK_INOTHER;
	case InOtherBlackList: return isself ? KKSG::ERR_BLACK_INOTHER : KKSG::ERR_BLACK_INSELF;
	case BlackNotExist: return KKSG::ERR_BLACK_NOTEXIST;
	case BlackRepeated: return KKSG::ERR_BLACK_REPEATED;
	case BlackMax: return KKSG::ERR_BLACK_MAX;
	case FriendSuccess: return KKSG::ERR_SUCCESS;
	case BlackSuccess: return KKSG::ERR_SUCCESS;
	default:return KKSG::ERR_UNKNOWN;
	}
}

bool FriendInfo::Update()
{
	int today = XCommon::GetTodayUnique();
	if (m_updatetime != today)	
	{
		m_degreetimes.clear();
		m_daydegree = 0;
		m_updatetime = today;
		m_receive = 0;
		m_receivetime = 0;
		m_send = 0;
		return true;
	}
	return false;
}

bool FriendInfo::Limit(UINT32 type, UINT32 count)
{
	auto i = m_degreetimes.find(type);
	if (i == m_degreetimes.end())
	{
		m_degreetimes[type] = 1;
		return false;
	}
	if (0 != count && count <= i->second)
	{
		return true;
	}
	++i->second;
	return false;
}

UINT32 FriendInfo::Add(UINT32 degree)
{
	m_daydegree += degree;
	if (m_degree + degree >= MAX_DEGREE)
	{
		degree = MAX_DEGREE > m_degree ? MAX_DEGREE - m_degree : 0;
	}
	m_degree += degree;
	return degree;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
bool Friend::SaveToDb(KKSG::Friend2Db& friend2db)
{
	m_record.Save(*friend2db.mutable_record());
	m_record.SetChanged(false);

	for (auto i = m_friends.begin(); i != m_friends.end(); ++i)
	{
		friend2db.add_roleid(i->first);
		friend2db.add_degree(i->second.m_degree);
		friend2db.add_daydegree(i->second.m_daydegree);
		for (auto j = i->second.m_degreetimes.begin(); j != i->second.m_degreetimes.end(); ++j)
		{
			KKSG::DegreeAddCount* counts = friend2db.add_counts();
			counts->add_type(j->first);
			counts->add_count(j->second);
		}
		friend2db.add_receive(i->second.m_receive);
		friend2db.add_send(i->second.m_send);
		friend2db.add_receiveall(i->second.m_receiveall);
		friend2db.add_receivetime(i->second.m_receivetime);
		friend2db.add_updatetime(i->second.m_updatetime);
	}	
	for (auto j = m_applys.begin(); j != m_applys.end(); ++j)
	{
		KKSG::FriendApply* apply = friend2db.add_apply();
		apply->set_roleid(j->roleid);
		apply->set_timestamp(j->timestamp);
	}
	for (auto k = m_blacklist.begin(); k != m_blacklist.end(); ++k)
	{
		friend2db.add_blacklist(*k);
	}
	for (auto i = m_noNotifyAddFriend.begin(); i != m_noNotifyAddFriend.end(); ++i)
	{
		friend2db.add_nonotifyrole(*i);
	}
	m_ischanged = false;
	return true;
}

bool Friend::LoadFromDb(const KKSG::Friend2Db& friend2db)
{
	if (friend2db.has_record())
	{
		m_record.Load(friend2db.record());
		m_record.Update();
	}

	for (int i = 0; i < friend2db.roleid_size(); ++i)
	{
		FriendInfo info(friend2db.roleid(i));
		if (i < friend2db.updatetime_size())
		{
			info.m_updatetime = friend2db.updatetime(i);
		}
		if (i < friend2db.degree_size())
		{
			info.m_degree = friend2db.degree(i);
		}
		if (i < friend2db.daydegree_size())
		{
			info.m_daydegree = friend2db.daydegree(i);
		}
		if (i < friend2db.counts_size())
		{
			for (int j = 0; j < friend2db.counts(i).type_size(); ++j)
			{
				info.m_degreetimes[friend2db.counts(i).type(j)] = friend2db.counts(i).count(j);
			}
		}
		if (i < friend2db.receive_size())
		{
			info.m_receive = friend2db.receive(i);
		}
		if (i < friend2db.send_size())
		{
			info.m_send = friend2db.send(i);
		}
		if (i < friend2db.receiveall_size())
		{
			info.m_receiveall = friend2db.receiveall(i);
		}
		if (i < friend2db.receivetime_size())
		{
			info.m_receivetime = friend2db.receivetime(i);
		}
		info.Update();
		m_friends[friend2db.roleid(i)] = info;
	}
	for (int j = 0; j < friend2db.apply_size(); ++j)
	{
		FriendApplyInfo info;
		info.roleid = friend2db.apply(j).roleid();
		info.timestamp = friend2db.apply(j).timestamp();
		m_applys.push_back(info);
	}
	for (int k = 0; k < friend2db.blacklist_size(); ++k)
	{
		m_blacklist.push_back(friend2db.blacklist(k));
	}
	for (int i = 0; i < friend2db.nonotifyrole_size(); ++i)
	{
		m_noNotifyAddFriend.push_back(friend2db.nonotifyrole(i));
	}
	return true;
}

UINT32 Friend::CanAddFriend(UINT64 friendroleid)
{
	if (GetFriendCount() >= MAX_FRIEND_COUNT)
	{
		return FriendMax;
	}
	if (IsBlack(friendroleid))
	{
		return InSelfBlackList;
	}
	auto i = m_friends.find(friendroleid);
	if (i != m_friends.end())
	{
		return FriendRepeated; 
	}
	return FriendSuccess;
}

bool Friend::InApply(UINT64 otherid)
{
	for (auto i = m_applys.begin(); i != m_applys.end(); ++i)
	{
		if (i->roleid == otherid)
		{
			return true;
		}
	}
	return false;
}

bool Friend::DeleteApply(UINT64 otherid)
{
	for (auto i = m_applys.begin(); i != m_applys.end(); ++i)
	{
		if (otherid == i->roleid)
		{
			m_applys.erase(i);
			m_ischanged = true;
			return true;
		}
	}
	return false;
}

bool Friend::AddApply(UINT64 otherid, UINT64& deleted)
{
	if (InApply(otherid))	
	{
		return false;
	}
	if (m_applys.size() >= MAX_APPLYFRIEND_COUNT)
	{
		deleted = m_applys.front().roleid;
		m_applys.pop_front();
	}
	FriendApplyInfo info;
	info.roleid = otherid;
	m_applys.push_back(info);
	m_ischanged = true;
	return true;
}

UINT32 Friend::AddFriend(UINT64 friendroleid)
{	
	FriendInfo info(friendroleid);
	m_friends[friendroleid] = info;

	m_ischanged = true;

	return FriendSuccess;
}

UINT32 Friend::RemoveFriend(UINT64 friendroleid)
{
	auto i = m_friends.find(friendroleid);
	if (i == m_friends.end())
	{
		return FriendNotExist;
	}
	m_friends.erase(i);

	m_ischanged = true;

	return FriendSuccess;
}

UINT32 Friend::GetDegree(UINT64 friendid)
{
	auto i = m_friends.find(friendid);
	return (i == m_friends.end()) ? 0 : i->second.m_degree;
}

UINT32 Friend::GetDayDegree(UINT64 friendid)
{
	auto i = m_friends.find(friendid);
	if (i != m_friends.end())
	{
		i->second.Update();
		return i->second.m_daydegree;
	}
	return 0;
}

UINT32 Friend::AddDegree(UINT64 selfid, UINT64 friendid, UINT32 degree, UINT32 type, UINT32 countlimit, bool islimit)
{
	auto i = m_friends.find(friendid);
	if (i != m_friends.end())
	{
		i->second.Update();
		if (islimit && i->second.Limit(type, countlimit))
		{
			return (UINT32)(-1);
		}
		i->second.Add(degree);
		m_ischanged = true;
		OnAddDegree(selfid, friendid, GetDegree(friendid));
		return 0;
	}
	return (UINT32)(-1);
}

void Friend::CleanDegree(UINT64 friendid)
{
	auto i = m_friends.find(friendid);
	if (i != m_friends.end())
	{
		i->second.m_degree = 0;
		i->second.m_daydegree = 0;
		i->second.m_degreetimes.clear();
	}
}

bool Friend::IsBlack(UINT64 otherroleid)
{
	for (auto i = m_blacklist.begin(); i != m_blacklist.end(); ++i)
	{
		if (*i == otherroleid)
		{
			return true;
		}
	}
	return false;
}

UINT32 Friend::AddBlackList(UINT64 otherroleid)
{
	if (IsBlack(otherroleid))
	{
		return BlackRepeated;
	}
	if (m_blacklist.size() > MAX_BLACK_COUNT)
	{
		//m_blacklist.pop_front();
		return BlackMax;
	}
	m_blacklist.push_back(otherroleid);
	m_ischanged = true;

	return BlackSuccess;
}

UINT32 Friend::RemoveBlackList(UINT64 otherroleid)
{
	for (auto i = m_blacklist.begin(); i != m_blacklist.end(); ++i)
	{
		if (*i == otherroleid)
		{
			m_blacklist.erase(i);
			m_ischanged = true;
			return BlackSuccess;
		}
	}
	return BlackNotExist;	
}

void Friend::ToFriendList(std::vector<UINT64>& ids)
{
	for (auto i = m_friends.begin(); i != m_friends.end(); ++i)
	{
		ids.push_back(i->first);
	}
}

void Friend::ToApplyList(std::vector<UINT64>& ids)
{
	for (auto i = m_applys.begin(); i != m_applys.end(); ++i)
	{
		ids.push_back(i->roleid);
	}
}

UINT32 Friend::GetReceiveAll(UINT64 otherid)
{
	auto i = m_friends.find(otherid);
	if (i != m_friends.end())
	{
		return i->second.m_receiveall;
	}
	return 0;
}

UINT32 Friend::GetReceiveTime(UINT64 otherid)
{
	auto i = m_friends.find(otherid);
	if (i != m_friends.end())
	{
		return i->second.m_receivetime;
	}
	return 0;
}

UINT32 Friend::GetReceive(UINT64 otherid)
{
	auto i = m_friends.find(otherid);
	if (i != m_friends.end())
	{
		i->second.Update();
		return i->second.m_receive;
	}
	return 0;
}

UINT32 Friend::GetSend(UINT64 otherid)
{
	auto i = m_friends.find(otherid);
	if (i != m_friends.end())
	{
		i->second.Update();
		return i->second.m_send;
	}
	return 0;
}

bool Friend::SetReceive(UINT64 otherid, UINT32 state)
{
	auto i = m_friends.find(otherid);
	if (i != m_friends.end())
	{
		i->second.Update();
		if (state != i->second.m_receive)
		{
			i->second.m_receive = state;
			if (state == KKSG::FriendGift_Received)
			{
				++i->second.m_receiveall;
				i->second.m_receivetime = (UINT32)time(NULL);
			}
			m_ischanged = true;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

bool Friend::SetSend(UINT64 otherid, UINT32 state)
{
	auto i = m_friends.find(otherid);
	if (i != m_friends.end())
	{
		i->second.Update();
		if (state != i->second.m_send)
		{
			m_ischanged = true;
			i->second.m_send = state;
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}

void Friend::AddNoNotifyAddFriend(UINT64 otherid)
{
	m_noNotifyAddFriend.push_back(otherid);
	m_ischanged = true;
}

void Friend::ClearNoNotifyAddFriend()
{
	m_noNotifyAddFriend.clear();
	m_ischanged = true;
}

bool Friend::Friend2Client(UINT64 selfid, UINT64 otherid, KKSG::Friend2Client* friend2client)
{
	friend2client->set_roleid(otherid);
	CRoleSummary* rolesummary = CRoleSummaryMgr::Instance()->GetRoleSummary(otherid);
	if (NULL == rolesummary)
	{
		return false;
	}

	const KKSG::RoleSummaryStored& brief = rolesummary->GetData();
	friend2client->set_name(rolesummary->GetName());
	friend2client->set_level(rolesummary->GetLevel());
	friend2client->set_profession(rolesummary->GetProfession());
	friend2client->set_powerpoint(rolesummary->GetPowerPoint());
	friend2client->set_nickid(brief.nickid());
	friend2client->set_titleid(rolesummary->GetTitleID());
	friend2client->set_viplevel(rolesummary->GetVipLevel());
	friend2client->set_paymemberid(rolesummary->GetPayMemberID());
	friend2client->set_openid(rolesummary->GetAccount());

	const PlatUserInfo* pPlatInfo = CPlatformFriendMgr::Instance()->GetPlatSelfInfo(rolesummary->GetAccount());
	if (pPlatInfo)
	{
		friend2client->set_nickname(pPlatInfo->nickname);
	}
	
	// degree
	FriendOp op(selfid);
	UINT32 degree = op.GetDegree(otherid);
	UINT32 daydegree = op.GetDayDegree(otherid);
	friend2client->set_daydegree(daydegree);
	friend2client->set_alldegree(degree);

	//gift
	friend2client->set_receiveall(op.GetReceiveAll(otherid));
	friend2client->set_receivetime(op.GetReceiveTime(otherid));
	friend2client->set_receivegiftstate(op.GetReceive(otherid));
	friend2client->set_sendgiftstate(op.GetSend(otherid));
	
	// lastlogin
	friend2client->set_lastlogin(rolesummary->GetLastLogout());

	// guild
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(otherid);
	if (NULL != guild)
	{
		friend2client->set_guildname(guild->GetName());
	}
	// mentor
	KKSG::EMentorRelationPosition pos = MentorMgr::Instance()->GetMentorPos(selfid, otherid);
	friend2client->set_mentortype(pos);

	return true;
}

void Friend::OnAddDegree(UINT64 selfID, UINT64 friendID, UINT32 degree)
{
	int pos = MentorMgr::Instance()->GetMentorPos(selfID, friendID);

	if (pos != KKSG::EMentorPosMaster)
	{
		return;
	}

	EventMgr::Instance()->AddEventOnlyOnMS(selfID, MENTOR_EVENT_OFFSET + KKSG::MentorTask_MentorIntimacy, 0, degree);
}
