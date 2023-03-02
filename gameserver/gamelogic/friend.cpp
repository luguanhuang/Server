#include "pch.h"
//#include "friend.h"
//#include "time.h"
//#include "pb/project.pb.h"
//#include "friendop.h"
//#include "gamelogic/friendconfig.h"
//#include "gamelogic/rolesummary.h"
//#include "gamelogic/rolesummarymgr.h"
//#include "unit/role.h"
//#include "unit/rolemanager.h"
//#include "gamelogic/friendrecord.h"
//#include "guild/guildmgr.h"
//#include "guild/guild.h"
//
//bool Friend::SaveToDb(KKSG::SocialInfo2Db* socialinfo2db)
//{
//	KKSG::Friend2Db friend2db;
//	for (auto i = m_friends.begin(); i != m_friends.end(); ++i)
//	{
//		friend2db.add_roleid(i->first);
//		friend2db.add_degree(i->second.m_degree);
//		friend2db.add_daydegree(i->second.m_daydegree);
//		for (auto j = i->second.m_degreetimes.begin(); j != i->second.m_degreetimes.end(); ++j)
//		{
//			KKSG::DegreeAddCount* counts = friend2db.add_counts();
//			counts->add_type(j->first);
//			counts->add_count(j->second);
//		}
//		friend2db.add_receive(i->second.m_receive);
//		friend2db.add_send(i->second.m_send);
//		friend2db.add_receiveall(i->second.m_receiveall);
//		friend2db.add_receivetime(i->second.m_receivetime);
//		friend2db.add_updatetime(i->second.m_updatetime);
//	}	
//	socialinfo2db->set_friend_(friend2db.SerializeAsString());
//
//	m_ischanged = false;
//	return true;
//}
//
//bool Friend::LoadFromDb(const KKSG::SocialInfo2Db* socialinfo2db)
//{
//	KKSG::Friend2Db friend2db;
//	friend2db.ParseFromString(socialinfo2db->friend_());
//	for (int i = 0; i < friend2db.roleid_size(); ++i)
//	{
//		FriendInfo info;
//		if (i < friend2db.updatetime_size())
//		{
//			info.m_updatetime = friend2db.updatetime(i);
//		}
//		if (i < friend2db.degree_size())
//		{
//			info.m_degree = friend2db.degree(i);
//		}
//		if (i < friend2db.daydegree_size())
//		{
//			info.m_daydegree = friend2db.daydegree(i);
//		}
//		if (i < friend2db.counts_size())
//		{
//			for (int j = 0; j < friend2db.counts(i).type_size(); ++j)
//			{
//				info.m_degreetimes[friend2db.counts(i).type(j)] = friend2db.counts(i).count(j);
//			}
//		}
//		if (i < friend2db.receive_size())
//		{
//			info.m_receive = friend2db.receive(i);
//		}
//		if (i < friend2db.send_size())
//		{
//			info.m_send = friend2db.send(i);
//		}
//		if (i < friend2db.receiveall_size())
//		{
//			info.m_receiveall = friend2db.receiveall(i);
//		}
//		if (i < friend2db.receivetime_size())
//		{
//			info.m_receivetime = friend2db.receivetime(i);
//		}
//		info.Update();
//		m_friends[friend2db.roleid(i)] = info;
//	}
//	return true;
//}
//
//UINT32 Friend::CanAddFriend(UINT64 friendroleid)
//{
//	if (GetFriendCount() >= MAX_FRIEND_COUNT)
//	{
//		return FriendMax;
//	}
//	if (IsBlack(friendroleid))
//	{
//		return InSelfBlackList;
//	}
//	auto i = m_friends.find(friendroleid);
//	if (i != m_friends.end())
//	{
//		return FriendRepeated; 
//	}
//	return FriendSuccess;
//}
//
//bool Friend::InApply(UINT64 otherid)
//{
//	for (auto i = m_applys.begin(); i != m_applys.end(); ++i)
//	{
//		if (i->roleid == otherid)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//bool Friend::DeleteApply(UINT64 otherid)
//{
//	for (auto i = m_applys.begin(); i != m_applys.end(); ++i)
//	{
//		if (otherid == i->roleid)
//		{
//			m_applys.erase(i);
//			m_ischanged = true;
//			return true;
//		}
//	}
//	return false;
//}
//
//bool Friend::AddApply(UINT64 otherid, UINT64& deleted)
//{
//	if (InApply(otherid))	
//	{
//		return false;
//	}
//	if (m_applys.size() >= MAX_APPLYFRIEND_COUNT)
//	{
//		deleted = m_applys.front().roleid;
//		m_applys.pop_front();
//	}
//	FriendApplyInfo info;
//	info.roleid = otherid;
//	m_applys.push_back(info);
//	m_ischanged = true;
//	return true;
//}
//
//UINT32 Friend::AddFriend(UINT64 friendroleid)
//{	
//	FriendInfo info;
//	m_friends[friendroleid] = info;
//
//	m_ischanged = true;
//
//	return FriendSuccess;
//}
//
//UINT32 Friend::RemoveFriend(UINT64 friendroleid)
//{
//	auto i = m_friends.find(friendroleid);
//	if (i == m_friends.end())
//	{
//		return FriendNotExist;
//	}
//	m_friends.erase(i);
//
//	m_ischanged = true;
//
//	return FriendSuccess;
//}
//
//UINT32 Friend::GetDegree(UINT64 friendid)
//{
//	auto i = m_friends.find(friendid);
//	return (i == m_friends.end()) ? 0 : i->second.m_degree;
//}
//
//UINT32 Friend::GetDayDegree(UINT64 friendid)
//{
//	auto i = m_friends.find(friendid);
//	if (i != m_friends.end())
//	{
//		i->second.Update();
//		return i->second.m_daydegree;
//	}
//	return 0;
//}
//
//UINT32 Friend::AddDegree(UINT64 friendid, UINT32 degree, UINT32 type, UINT32 countlimit, bool islimit)
//{
//	auto i = m_friends.find(friendid);
//	if (i != m_friends.end())
//	{
//		i->second.Update();
//		if (islimit && i->second.Limit(type, countlimit))
//		{
//			return (UINT32)(-1);
//		}
//		i->second.Add(degree);
//		m_ischanged = true;
//		return 0;
//	}
//	return (UINT32)(-1);
//}
//
//bool Friend::IsBlack(UINT64 otherroleid)
//{
//	for (auto i = m_blacklist.begin(); i != m_blacklist.end(); ++i)
//	{
//		if (*i == otherroleid)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//UINT32 Friend::AddBlackList(UINT64 otherroleid)
//{
//	if (IsBlack(otherroleid))
//	{
//		return BlackSuccess;
//	}
//	if (m_blacklist.size() > MAX_BLACK_COUNT)
//	{
//		//m_blacklist.pop_front();
//		return BlackMax;
//	}
//	m_blacklist.push_back(otherroleid);
//	m_ischanged = true;
//
//	return BlackSuccess;
//}
//
//UINT32 Friend::RemoveBlackList(UINT64 otherroleid)
//{
//	for (auto i = m_blacklist.begin(); i != m_blacklist.end(); ++i)
//	{
//		if (*i == otherroleid)
//		{
//			m_blacklist.erase(i);
//			m_ischanged = true;
//			return BlackSuccess;
//		}
//	}
//	return BlackNotExist;	
//}
//
//void Friend::ToFriendList(std::vector<UINT64>& ids)
//{
//	for (auto i = m_friends.begin(); i != m_friends.end(); ++i)
//	{
//		ids.push_back(i->first);
//	}
//}
//
//void Friend::ToApplyList(std::vector<UINT64>& ids)
//{
//	for (auto i = m_applys.begin(); i != m_applys.end(); ++i)
//	{
//		ids.push_back(i->roleid);
//	}
//}
//
//UINT32 Friend::GetReceiveAll(UINT64 otherid)
//{
//	auto i = m_friends.find(otherid);
//	if (i != m_friends.end())
//	{
//		return i->second.m_receiveall;
//	}
//	return 0;
//}
//
//UINT32 Friend::GetReceiveTime(UINT64 otherid)
//{
//	auto i = m_friends.find(otherid);
//	if (i != m_friends.end())
//	{
//		return i->second.m_receivetime;
//	}
//	return 0;
//}
//
//UINT32 Friend::GetReceive(UINT64 otherid)
//{
//	auto i = m_friends.find(otherid);
//	if (i != m_friends.end())
//	{
//		i->second.Update();
//		return i->second.m_receive;
//	}
//	return 0;
//}
//
//UINT32 Friend::GetSend(UINT64 otherid)
//{
//	auto i = m_friends.find(otherid);
//	if (i != m_friends.end())
//	{
//		i->second.Update();
//		return i->second.m_send;
//	}
//	return 0;
//}
//
//bool Friend::SetReceive(UINT64 otherid, UINT32 state)
//{
//	auto i = m_friends.find(otherid);
//	if (i != m_friends.end())
//	{
//		i->second.Update();
//		if (state != i->second.m_receive)
//		{
//			i->second.m_receive = state;
//			if (state == KKSG::FriendGift_Received)
//			{
//				++i->second.m_receiveall;
//				i->second.m_receivetime = (UINT32)time(NULL);
//			}
//			m_ischanged = true;
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//	return false;
//}
//
//bool Friend::SetSend(UINT64 otherid, UINT32 state)
//{
//	auto i = m_friends.find(otherid);
//	if (i != m_friends.end())
//	{
//		i->second.Update();
//		if (state != i->second.m_send)
//		{
//			m_ischanged = true;
//			i->second.m_send = state;
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//	return false;
//}
//
//bool Friend::Friend2Client(UINT64 selfid, UINT64 otherid, KKSG::Friend2Client* friend2client)
//{
//	friend2client->set_roleid(otherid);
//	RoleSummary* rolesummary = RoleSummaryMgr::Instance()->FindRoleSummary(otherid);
//	if (NULL == rolesummary)
//	{
//		return false;
//	}
//	friend2client->set_level(rolesummary->dwRoleLevel);
//	friend2client->set_viplevel(rolesummary->viplevel);
//	friend2client->set_powerpoint(rolesummary->dwPowerPoint);
//	friend2client->set_name(rolesummary->strName);
//	friend2client->set_profession(rolesummary->dwProfession);
//	//friend2client->set_audioid(rolesummary->audioid);
//	friend2client->set_nickid(rolesummary->nickid);
//	
//	// degree
//	FriendOp op(selfid);
//	UINT32 degree = op.GetDegree(otherid);
//	UINT32 daydegree = op.GetDayDegree(otherid);
//	friend2client->set_daydegree(daydegree);
//	friend2client->set_alldegree(degree);
//
//	//gift
//	friend2client->set_receiveall(op.GetReceiveAll(otherid));
//	friend2client->set_receivetime(op.GetReceiveTime(otherid));
//	friend2client->set_receivegiftstate(op.GetReceive(otherid));
//	friend2client->set_sendgiftstate(op.GetSend(otherid));
//	
//	// lastlogin
//	UINT32 nowtime = (UINT32)time(NULL);
//	UINT32 timelastlogin = rolesummary->timelastlogin;
//	if (Logoff == RoleManager::Instance()->GetRoleState(rolesummary->qwRoleID))
//	{
//		friend2client->set_lastlogin(nowtime > timelastlogin ? nowtime - timelastlogin : 0);
//	}
//	else
//	{
//		friend2client->set_lastlogin(0);
//	}
//
//	// guild
//	Guild* guild = GuildMgr::Instance()->FindGuildByRoleId(otherid);
//	if (NULL != guild)
//	{
//		friend2client->set_guildname(guild->GetName());
//	}
//	return true;
//}
//
//void Friend::ShowBlackList( std::string& str )
//{
//	str = "";
//	for( auto i=m_blacklist.begin(); i!= m_blacklist.end(); i++ )
//	{
//		char buf[64];
//		memset(buf, 0, sizeof(buf));
//		sprintf( buf, "%llu", *i );
//		str += "#";
//		str += buf;
//	}
//}
