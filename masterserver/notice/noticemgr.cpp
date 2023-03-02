#include "pch.h"
#include "noticemgr.h"
#include "network/gatelink.h"
#include "foreach.h"
#include "notice.h"
#include "mail/maildata.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "social/flower.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"

INSTANCE_SINGLETON(NoticeMgr);

NoticeMgr::NoticeMgr()
{

}

NoticeMgr::~NoticeMgr()
{

}

bool NoticeMgr::Init()
{
	if(!LoadFile())
	{
		return false;
	}

	m_timehandle = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);

	return true;
}

void NoticeMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timehandle);
}

bool NoticeMgr::CheckFile()
{
	NoticeTable tmpNoticeTable;
	if (!tmpNoticeTable.LoadFile("table/Notice.txt"))
	{
		SSWarn<<"load file table/Notice.txt failed"<<END;
		return false;
	}

	return true;
}

bool NoticeMgr::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_table.LoadFile("table/Notice.txt");

	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		NoticeTable::RowData* rowdata = *i;

		bool haslink = false;
		m_id2paramtypes[rowdata->id] = ParseParamTypes(rowdata->info, haslink);
		if (haslink)
		{
			NoticeLinkInfo linkinfo;
			linkinfo.content = rowdata->linkcontent;
			linkinfo.param = rowdata->linkparam;
			m_id2link[rowdata->id] = linkinfo;
		}

		if (rowdata->timebegin.size() == 6 && rowdata->timeend.size() == 6 && rowdata->timespan > 0)
		{
			NoticeTime time;
			time.timebegin.tm_hour = rowdata->timebegin[3];
			time.timebegin.tm_min = rowdata->timebegin[4];
			time.timebegin.tm_sec = rowdata->timebegin[5];
			if (rowdata->timebegin[0] != 0 && rowdata->timebegin[1] != 0  && rowdata->timebegin[2] != 0)
			{
				time.hasbegin = true;
				time.timebegin.tm_year = rowdata->timebegin[0];
				time.timebegin.tm_mon = rowdata->timebegin[1];
				time.timebegin.tm_mday = rowdata->timebegin[2];
			}

			time.timeend.tm_hour = rowdata->timeend[3];
			time.timeend.tm_min = rowdata->timeend[4];
			time.timeend.tm_sec = rowdata->timeend[5];
			if (rowdata->timeend[0] != 0 && rowdata->timeend[1] != 0  && rowdata->timeend[2] != 0)
			{
				time.hasend = true;
				time.timeend.tm_year = rowdata->timeend[0];
				time.timeend.tm_mon = rowdata->timeend[1];
				time.timeend.tm_mday = rowdata->timeend[2];
			}
			time.timespan = rowdata->timespan;
			AddTimeNotice(rowdata->id, time);
		}
	}

	return true;
}

void NoticeMgr::ClearFile()
{
	m_table.Clear();

	m_id2time.clear();
	m_id2link.clear();
	m_id2paramtypes.clear();
}

void NoticeMgr::AddTimeNotice(UINT32 id, NoticeTime& time)
{
	m_id2time[id] = time;
}

const std::string* NoticeMgr::GetInfo(UINT32 id)
{
	NoticeTable::RowData* rowdata = m_table.GetByid(id);
	if (NULL == rowdata)
	{
		return NULL;
	}
	return &rowdata->info;
}

UINT32 NoticeMgr::GetChannel(UINT32 index)
{
	NoticeTable::RowData* rowdata = m_table.GetByid(index);
	if (NULL == rowdata)
	{
		return KKSG::LampShortChannel;
	}
	return rowdata->channel;
}

UINT32 NoticeMgr::GetPriority(UINT32 index)
{
	NoticeTable::RowData* rowdata = m_table.GetByid(index);
	if (NULL == rowdata)
	{
		return 0;
	}
	return rowdata->priority;
}

UINT32 NoticeMgr::GetTimeSpan(UINT32 index)
{
	NoticeTable::RowData* rowdata = m_table.GetByid(index);
	if (NULL == rowdata)
	{
		return 0;
	}
	return rowdata->timespan;
}

UINT32 NoticeMgr::GetLevel(UINT32 index)
{
	NoticeTable::RowData* rowdata = m_table.GetByid(index);
	if (NULL == rowdata)
	{
		return 0;
	}
	return rowdata->level;
}

NoticeLinkInfo* NoticeMgr::GetLinkInfo(UINT32 id)
{
	auto i = m_id2link.find(id);
	if (i == m_id2link.end())
	{
		return NULL;
	}
	return &i->second;
}

std::vector<NoticeParamType>* NoticeMgr::GetParamTypes(UINT32 id)
{
	auto i = m_id2paramtypes.find(id);
	if (i == m_id2paramtypes.end())
	{
		return NULL;
	}
	return &(i->second);
}

void NoticeMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	time_t nowtime = time(NULL);
	tm* nowtimenode = localtime(&nowtime);

	for (auto i = m_id2time.begin(); i != m_id2time.end(); ++i)
	{
		NoticeTime& timenode = i->second;
		if (IsEnd(timenode, nowtimenode, nowtime))
		{
			timenode.isprocess = false;
			auto j = m_id2timenotice.find(i->first);
			if (j != m_id2timenotice.end())
			{
				m_id2timenotice.erase(j);
			}
		}
		else
		{
			if (!timenode.isprocess && IsBegin(timenode, nowtimenode, nowtime))
			{
				timenode.isprocess = true;
				auto j = m_id2timenotice.find(i->first);
				if (j == m_id2timenotice.end())
				{
					m_id2timenotice[i->first] = nowtime;
				}
			}
		}
	}
	for (auto i = m_id2timenotice.begin(); i != m_id2timenotice.end(); ++i)
	{
		if (i->second <= nowtime)
		{
			TimeNotice(i->first);
			i->second = (nowtime + GetSpan(i->first));
		}
	}

	//handle notice from idip
	for (auto i = m_Idipid2time.begin(); i != m_Idipid2time.end();)
	{
		NoticeTime& timenode = i->second.first;
		if (IsEnd(timenode, nowtimenode, nowtime))
		{
			timenode.isprocess = false;
			auto j = m_Idiptimenotice.find(i->first);
			if (j != m_Idiptimenotice.end())
			{
				m_Idiptimenotice.erase(j);
			}
			m_Idipid2time.erase(i++);
		}
		else
		{
			if (!timenode.isprocess && IsBegin(timenode, nowtimenode, nowtime))
			{
				timenode.isprocess = true;
				auto j = m_Idiptimenotice.find(i->first);
				if (j == m_Idiptimenotice.end())
				{
					m_Idiptimenotice[i->first] = nowtime;
				}
			}
			i ++;
		}
	}

	for (auto i = m_Idiptimenotice.begin(); i != m_Idiptimenotice.end(); ++i)
	{
		if (i->second <= nowtime)
		{
			auto j = m_Idipid2time.find(i->first);
			if (j != m_Idipid2time.end())
			{
				SendNotice(j->second.second.uChannel, j->second.second.uPriority, j->second.second.szContent);
				i->second = nowtime + j->second.first.timespan;
			}
		}
	}
}

bool NoticeMgr::IsBegin(const NoticeTime& timenode, const tm* nowtimenode, time_t nowtime)
{
	tm temptimenode = *nowtimenode;	
	temptimenode.tm_hour = timenode.timebegin.tm_hour;
	temptimenode.tm_min = timenode.timebegin.tm_min;
	temptimenode.tm_sec = timenode.timebegin.tm_sec;
	if (timenode.hasbegin)
	{
		temptimenode.tm_year = timenode.timebegin.tm_year - 1900;
		temptimenode.tm_mon = timenode.timebegin.tm_mon - 1;
		temptimenode.tm_mday = timenode.timebegin.tm_mday;
	}
	time_t begintime = mktime(&temptimenode);
	if (-1 == begintime)
	{
		return false;
	}
	return begintime <= nowtime;
}

bool NoticeMgr::IsEnd(const NoticeTime& timenode, const tm* nowtimenode, time_t nowtime)
{
	tm temptimenode = *nowtimenode;	
	temptimenode.tm_hour = timenode.timeend.tm_hour;
	temptimenode.tm_min = timenode.timeend.tm_min;
	temptimenode.tm_sec = timenode.timeend.tm_sec;
	if (timenode.hasend)
	{
		temptimenode.tm_year = timenode.timeend.tm_year - 1900;
		temptimenode.tm_mon = timenode.timeend.tm_mon - 1;
		temptimenode.tm_mday = timenode.timeend.tm_mday;
	}
	time_t endtime = mktime(&temptimenode);
	if (-1 == endtime)
	{
		return false;
	}
	return endtime <= nowtime;
}

UINT32 NoticeMgr::GetSpan(UINT32 id)
{
	auto i = m_id2time.find(id);
	if (i != m_id2time.end())
	{
		return i->second.timespan;
	}
	return 0;
}

void NoticeMgr::TimeNotice(UINT32 id)
{
	Notice notice(id);
	notice.Send();
}

void NoticeMgr::LevelSealNotice(UINT32 ID)
{
	m_id2timenotice.erase(ID);
	NoticeTime notice = HandlerSpan(ID, 3);
	AddTimeNotice(ID, notice);
}

NoticeTime NoticeMgr::HandlerSpan(UINT32 id, UINT32 count)//²¥·Å´ÎÊý
{
	time_t begin = time(NULL) - GetTimeSpan(id) + 6;
	time_t end = begin + GetTimeSpan(id) * count;
	tm* begintime = localtime(&begin);
	tm b = *begintime;
	tm* endtime = localtime(&end);
	NoticeTime noticetime(b, *endtime, GetTimeSpan(id));
	return noticetime;
}

void NoticeMgr::SendNotice(UINT32 uChannel, UINT32 uPro, std::string content)
{
	Notice notice(uChannel, uPro, content);
	notice.Send();
}

bool NoticeMgr::AddIdipNotice(UINT32 id, stNoticeContent notice, NoticeTime timenode)
{
	auto it = m_Idipid2time.find(id);
	if (it == m_Idipid2time.end())
	{
		m_Idipid2time.insert(std::make_pair(id, std::make_pair(timenode, notice)));
		return true;
	}
	return false;
}

bool NoticeMgr::DelIdipNotice(UINT32 id)
{
	auto i = m_Idipid2time.find(id);
	if (i == m_Idipid2time.end())
	{
		return false;
	}
	m_Idipid2time.erase(i);

	auto j = m_Idiptimenotice.find(id);
	if (j == m_Idiptimenotice.end())
	{
		return false;
	}
	m_Idiptimenotice.erase(j);
	return true;
}

void NoticeMgr::GetIdipNotice(std::vector<std::pair<NoticeTime, stNoticeContent>>& list)
{
	for (auto i = m_Idipid2time.begin(); i != m_Idipid2time.end(); i ++)
	{
		list.push_back(i->second);
	}
}

void NoticeMgr::TShowTop3Notice(CRoleSummary* srcrole, UINT32 rank)
{
	Notice notice(TshowTop3Notice);
	notice.AddRole(srcrole);
	//notice.Add(rank);
	notice.Send();
}

std::vector<NoticeParamType> NoticeMgr::ParseParamTypes(const std::string& info, bool& haslink)
{
	std::vector<NoticeParamType> paramtypes;
	for (UINT32 i = 0; i < info.size(); ++i)
	{
		if (GetStringIndex(info, i) == '$' && GetStringIndex(info, i - 1) != '\\')
		{
			switch (GetStringIndex(info, i + 1))
			{
			case 'R':
				{
					paramtypes.push_back(ParamRole);
					break;
				}
			case 'I':
				{
					paramtypes.push_back(ParamItem);
					break;
				}
			case 'N':
				{
					paramtypes.push_back(ParamNum);
					break;
				}
			case 'G':
				{
					paramtypes.push_back(ParamGuild);
					break;
				}
			case 'C':
				{
					paramtypes.push_back(ParamContent);
					break;
				}
			case 'L':
				{
					paramtypes.push_back(ParamLink);
					haslink = true;
					break;
				}
			case 'D':
				{
					paramtypes.push_back(ParamDragonGuild);
					break;
				}
			default:
				break;
			}
		}
	}
	return paramtypes;
}

char NoticeMgr::GetStringIndex(const std::string& info, UINT32 index)
{
	return index >= info.size() ? ' ' : info.at(index);
}


void NoticeMgr::SendFlowerNotice(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count)
{
	pFlowerNoticeFunc func = GetSendFlowerNoticeFunc(itemID, count);
	if (func)
	{
		(NoticeMgr::Instance()->*func)(srcrole, destrole, itemID, count);
	}
}

void NoticeMgr::FlowerNotice(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count)
{
	Notice notice(FlowerNotice1);
	notice.AddRole(srcrole);
	notice.AddItem(itemID, count);
	notice.SendRole(destrole->GetID());
}

void NoticeMgr::FlowerNotice2(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count)
{
	Notice notice(FlowerNotice_2);
	notice.AddRole(srcrole);
	notice.AddRole(destrole);
	notice.AddItem(itemID, count);
	notice.Send();
}

void NoticeMgr::FlowerNotice3(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count)
{
	Notice notice(FlowerNotice_3);
	notice.AddRole(srcrole);
	notice.AddRole(destrole);
	notice.AddItem(itemID, count);
	notice.AddRole(destrole);
	notice.Send();
}

void NoticeMgr::FlowerNotice4(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count)
{
	Notice notice(FlowerNotice_4);
	notice.AddRole(srcrole);
	notice.AddRole(destrole);
	notice.AddItem(itemID, count);
	notice.AddRole(destrole);
	notice.Send();
}

void NoticeMgr::FlowerNotice5(CRoleSummary* srcrole, CRoleSummary* destrole, UINT32 itemID, UINT32 count)
{
	Notice notice(FlowerNotice_5);
	notice.AddRole(srcrole);
	notice.AddRole(destrole);
	notice.AddItem(itemID, count);
	notice.AddRole(destrole);
	notice.Send();
}

NoticeMgr::pFlowerNoticeFunc NoticeMgr::GetSendFlowerNoticeFunc(UINT32 itemID, UINT32 count)
{
	const FlowerSendNoticeTable::RowData* data = FlowerConfig::Instance()->GetNoticeData(itemID, count);
	if (!data)
	{
		return NULL;
	}
	switch(data->NoticeID)
	{
	case FlowerNotice1:
		return &NoticeMgr::FlowerNotice;
	case FlowerNotice_2:
		return &NoticeMgr::FlowerNotice2;
	case FlowerNotice_3:
		return &NoticeMgr::FlowerNotice3;
	case FlowerNotice_4:
		return &NoticeMgr::FlowerNotice4;
	case FlowerNotice_5:
		return &NoticeMgr::FlowerNotice5;
	default:
		return NULL;
	}
	return NULL;
}

void NoticeMgr::FlowerValentinesFirstNotice(CRoleSummary* role)
{
	Notice notice(FlowerValentinesDayNoticeFirst);
	notice.AddRole(role);
	notice.Send();
}

void NoticeMgr::FlowerValentinesOtherNotice(CRoleSummary* role, UINT32 rank)
{
	Notice notice(FlowerValentinesDayNoticeOther);
	notice.AddRole(role);
	notice.Add(rank);
	notice.Send();
}

void NoticeMgr::Flower520FirstNotice(CRoleSummary* role)
{
	Notice notice(Flower520DayNoticeFirst);
	notice.AddRole(role);
	notice.Send();
}

void NoticeMgr::Flower520OtherNotice(CRoleSummary* role, UINT32 rank)
{
	Notice notice(Flower520DayNoticeOther);
	notice.AddRole(role);
	notice.Add(rank);
	notice.Send();
}

void NoticeMgr::FirstPassNotice(int serverid, std::vector<CRoleSummary*>& roles, const std::string& name , int rank)
{
	Notice notice(FirstPassStage);

	string roleNames;
	std::stringstream ss;
	ss << rank;
	for (int i = 0; i < roles.size(); ++i)
	{
		roleNames += roles[i]->GetName();
		if (i != (roles.size() - 1))
		{
			roleNames += ",";
		}
	}
	notice.Replace("$R", roleNames);
	notice.Replace("$C", name);
	notice.Replace("$N", ss.str());

	notice.SendServerID(serverid);
}


void NoticeMgr::SendAddFriendNotice(UINT64 roleid, CRoleSummary* friendrole)
{
	Notice notice(AddFriendNotice);

	notice.AddRole(friendrole);

	notice.SendRole(roleid);
}

void NoticeMgr::NoticeGuildBossLvlup(UINT64 guildId)
{
	Notice notice(GuildBossLvlup);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildBossDps(std::string roleName, UINT64 guildId)
{
	Notice notice(GuildBossDps);
	notice.Replace("$R", roleName);
	notice.SendGuild(guildId);
}

void NoticeMgr::GuildArenaOutWar(UINT64 roleId)
{
	Notice notice(GuildArenaPushOut);
	notice.SendRole(roleId);
}

void NoticeMgr::GuildArenaOnWar(UINT64 roleId)
{
	Notice notice(GuildArenaPushOn);
	notice.SendRole(roleId);
}

void NoticeMgr::NoticeGuildArenaReady(UINT64 guildId)
{
	Notice notice(GuildArenaReady);
	notice.AddLink();
	notice.SendGuild(guildId);

}

void NoticeMgr::NoticeGuildArenaStart()
{
	Notice notice(GuildArenaStart);
	notice.AddLink();
	notice.Send();
}

void NoticeMgr::NoticeGuildApply()
{
	Notice notice(GuildArenaApply);
	notice.AddLink();
	notice.Send();
}

void NoticeMgr::NoticeGuildArenaYourTurn(UINT64 roleID)
{
	Notice notice(GuildArenaYouTurn);
	notice.SendRole(roleID);
}

void NoticeMgr::NoticeGuildArenaKickYou(UINT64 roleID)
{
	Notice notice(GuildArenaKickYou);
	notice.SendRole(roleID);
}

void NoticeMgr::NoticeGuildArenaBattleOne()
{
	Notice notice(GuildArenaBattleOne);
	notice.AddLink();
	notice.Send();
}

void NoticeMgr::NoticeGuildArenaEnterSceneTwo(UINT64 guildId)
{
	Notice notice(GuildArenaEnterSceneTwo);
	notice.AddLink();
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildArenaEnterSceneThree(UINT64 guildId)
{
	Notice notice(GuildArenaEnterSceneThree);
	notice.AddLink();
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildLeaderOpenQA(UINT64 guildId)
{
	Notice notice(GuildQALeftTime_QAOpen);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeJoinWorldQA(UINT32 min)
{
	std::string strMin = ToString(min);
	Notice notice(TakeWorldQANotice);
	notice.Replace("$R", strMin);
	notice.Send();
}

void NoticeMgr::NoticeJoinGuildQA(UINT64 guildId, UINT32 min)
{
	std::string strMin = ToString(min);
	Notice notice(TakeGuildQANotice);
	notice.Replace("$R", strMin);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildArenaEnterSceneOne(UINT64 guildId)
{
	Notice notice(GuildArenaEnterSceneOne);
	notice.AddLink();
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildArenaBattleTwo()
{
	Notice notice(GuildArenaBattleTwo);
	notice.AddLink();
	notice.Send();
}
void NoticeMgr::NoticeGuildArenaBattleFin()
{
	Notice notice(GuildArenaBattleFin);
	notice.AddLink();
	notice.Send();
}

void NoticeMgr::NoticeGuildArenaWin(UINT64 guildId, UINT64 losId)
{
	Guild* pGuildLos = CGuildMgr::Instance()->GetGuild(losId);
	if (pGuildLos == NULL)
	{
		return ;
	}

	Guild* pGuildWin = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuildWin == NULL)
	{
		return ;
	}

	Notice notice(GuildArenaBattleWin);
	notice.Replace("$G", pGuildLos->GetName());
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildArenaLos(UINT64 guildId, UINT64 winId)
{
	Guild* pGuildWin = CGuildMgr::Instance()->GetGuild(winId);
	if (pGuildWin == NULL)
	{
		return ;
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return ;
	}

	Notice notice(GuildArenaBattleLos);
	notice.Replace("$G", pGuildWin->GetName());
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildArenaCom(UINT64 guildId, UINT64 losId)
{
	Guild* pGuildLos = CGuildMgr::Instance()->GetGuild(losId);
	if (pGuildLos == NULL)
	{
		return ;
	}

	Guild* pGuildWin = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuildWin == NULL)
	{
		return ;
	}

	Notice notice(GuildArenaBattleCom);
	notice.Replace("$G", pGuildLos->GetName());
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildArenaWiner(UINT64 guildId, UINT32 type)
{
	Guild* pGuildWin = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuildWin == NULL)
	{
		return ;
	}
	CRoleSummary* pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(pGuildWin->GetLeaderID());
	if (pRole == NULL)
	{
		return ;
	}

	if (type == GuildArenaWiner)
	{
		Notice notice(GuildArenaWiner);
		notice.Replace("$R", pRole->GetName());
		notice.Replace("$G", pGuildWin->GetName());
		notice.Send();
	}
	else if (type == GuildArenaWinerTwo)
	{
		Notice notice(GuildArenaWinerTwo);
		notice.Replace("$R", pRole->GetName());
		notice.Replace("$G", pGuildWin->GetName());
		notice.Send();
	}
	else if (type == GuildArenaWinerThree)
	{
		Notice notice(GuildArenaWinerThree);
		notice.Replace("$R", pRole->GetName());
		notice.Replace("$G", pGuildWin->GetName());
		notice.Send();
	}
}

void NoticeMgr::GuildArenaBeforeNotice(UINT64 guildId, UINT32 type)
{
	if (type == 1)
	{
		Notice notice(GuildArenaBefore3);
		notice.SendGuild(guildId);
	}
	else if (type == 2)
	{
		Notice notice(GuildArenaBefore2);
		notice.SendGuild(guildId);
	}
	else
	{
		Notice notice(GuildArenaBefore1);
		notice.SendGuild(guildId);
	}
}

void NoticeMgr::NoticeGuildArenaInit()
{
	Notice notice(GuildArenaInit);
	notice.AddLink();
	notice.Send();
}

void NoticeMgr::NoticeGuildSkillUp(std::string skillName, UINT32 skillLvl, UINT64 guildId)
{
	Notice notice(GuildStudySkillUp);
	notice.Replace("$R", skillName);
	std::string dest = ToString(skillLvl);
	notice.Replace("$N", dest);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildDareFinished(const std::set<UINT64>& roleSet, const std::string sceneName, UINT64 guildId, UINT32 exp)
{
	std::string  rolenames;
	for (auto i = roleSet.begin(); i != roleSet.end(); ++i)
	{
		CRoleSummary* pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(*i);
		if (pRole != NULL)
		{
			rolenames += pRole->GetName();
		}
		if (i != roleSet.end())
		{
			rolenames += "/";
		}
	}

	Guild* pGuild = CGuildMgr::Instance()->GetGuild(guildId);
	if (pGuild == NULL)
	{
		return ;
	}
	/*
	UINT32 guildExp = CGuildConfig::Instance()->GetDareGuildExp(pGuild->GetLevel());
	if (guildExp <= 0)
	{
		return;
	}
	*/
	std::string destExp = ToString(exp);

	CRoleSummary* pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(pGuild->GetLeaderID());
	if (pRole == NULL)
	{
		return ;
	}

	Notice notice(GuildDareFinished);
	//notice.AddSrc(pRole);
	notice.Replace("$R", rolenames);
	notice.Replace("$N", sceneName);
	//notice.Replace("$K", destExp);
	notice.SendGuild(guildId);
}

void NoticeMgr::GetGuildBonusNotice(CRoleSummary* source, CRoleSummary* srcrole, UINT64 guildID, UINT32 itemID, UINT32 count)
{
	Notice notice(GuildBonusNotice);
	notice.AddRole(srcrole);
	notice.AddItem(itemID, count);
	notice.SendGuild(guildID);
}

void NoticeMgr::FirstGetGuildBonusNotice(/*CRoleSummary* source*/std::string& bonusName, CRoleSummary* srcrole, UINT64 guildID, UINT32 itemID, UINT32 count)
{
	Notice notice(FirstGuildBonusNotice);
	notice.AddRole(srcrole);
	notice.Replace("$C", bonusName);
	notice.AddItem(itemID, count);
	notice.SendGuild(guildID);
}

void NoticeMgr::LuckestGetGuildBonusNotice(CRoleSummary* source, const std::string& names, CRoleSummary* scrole, UINT32 itemID, UINT32 count, UINT64 guildID)
{
	Notice notice(LuckestGuildBonusNotice);
	notice.Replace("$C", names);
	notice.AddRole(scrole);
	notice.AddItem(itemID, count);
	notice.SendGuild(guildID);
}

void NoticeMgr::SendGuildBonusNotice(CRoleSummary* source, UINT32 itemID, UINT32 count, UINT64 guildID)
{
	Notice notice(SendGuildBonusNotice1);
	notice.AddRole(source);
	notice.AddItem(itemID, count);
	notice.SendGuild(guildID);
}

void NoticeMgr::SendGuildBonusNoticeOnTime(UINT64 guildID)
{
	Notice notice(SendGuildBonusNotice2);
	notice.SendGuild(guildID);
}

void NoticeMgr::NoticeGuildActivityLeader(const std::string name, const std::string nameTwo, UINT64 guildId)
{
	Notice notice(GuildActivityLeader);
	notice.Replace("$R", name);
	notice.Replace("$X", nameTwo);
	notice.SendGuild(guildId);

}

void NoticeMgr::PvpNotice( bool beginOrEnd )
{
	if(beginOrEnd)
	{
		Notice notice(PvpBeginNotice);
		notice.Send();
	}
	else
	{
		Notice notice(PvpEndNotice);
		notice.Send();
	}
}

void NoticeMgr::NoticeGuildLadderReady()
{
	Notice notice(GuildLadderReady);
	notice.Send();
}

void NoticeMgr::NoticeGuildLadderBegin(UINT64 guildId)
{
	Notice notice(GuildLadderBegin);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildLadderWin(UINT64 guildId, UINT64 roleId)
{
	CRoleSummary* pData = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pData == NULL)
	{
		return ;
	}

	Notice notice(GuildLadderWin);
	notice.Replace("$R", pData->GetName());
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildLadderWinTimes(UINT64 guildId, UINT64 roleId, UINT32 winTimes)
{
	CRoleSummary* pData = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pData == NULL)
	{
		return ;
	}

	Notice notice(GuildLadderWinTimes);
	notice.Replace("$R", pData->GetName());
	notice.Replace("$X", ToString(winTimes));
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildLadderTen(UINT64 guildId, UINT32 rank)
{
	Notice notice(GuildLadderTen);
	notice.Replace("$X", ToString(rank));
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildLadderFive(UINT64 guildId, UINT32 rank)
{
	Notice notice(GuildLadderFive);
	notice.Replace("$X", ToString(rank));
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildLadderEnd(UINT64 guildId, UINT32 rank, std::set<UINT64>& roleSet)
{
	bool isFirst = true;
	std::string names;
	foreach (i in roleSet)
	{
		if (!isFirst)
		{
			names += ",";
		}
		CRoleSummary* pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(*i);
		if (pRole != NULL)
		{
			names += pRole->GetName();
		}
		isFirst = false;
	}

	Notice notice(GuildLadderEnd);
	notice.Replace("$R", ToString(rank));
	notice.Replace("$X", names);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildCardMatch(UINT64 guildid, UINT32 type, UINT32 round)
{
	Notice notice(type);
	if (type == GuildCardRoundNotice)
	{
		notice.Add(round);
	}
	notice.SendGuild(guildid);
}

void NoticeMgr::NoticeGuildRankEnd(std::vector<std::string>& roleVec)
{
	Notice notice(GuildRankEnd);
	std::map<UINT32, std::string> roleMap;
	auto iter = roleVec.begin();
	for (UINT32 cnt = 1; cnt <= 5; ++cnt)
	{
		if (iter == roleVec.end())
		{
			roleMap[cnt] = "";
		}
		else
		{
			roleMap[cnt] = *iter;
			++iter;
		}
	}

	notice.Replace("$R", roleMap[1]);
	notice.Replace("$X", roleMap[2]);
	notice.Replace("$T", roleMap[3]);
	notice.Replace("$Y", roleMap[4]);
	notice.Replace("$N", roleMap[5]);
    notice.Send();
}

void NoticeMgr::NoticeGuildRankReward(UINT64 guildId, UINT32 rank)
{
	Notice notice(GuildRankReward);
	notice.Replace("$R", ToString(rank));
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildCardFirst(UINT64 guildId, CRoleSummary* role)
{
	Notice notice(GuildCardFirstNotice);
	notice.AddRole(role);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildCardBeginSoon()
{
	Notice notice(GuildCardBeginSoonNotice);
	notice.Send();
}

void NoticeMgr::NoticePokerTournamentBeginSoon()
{
	Notice notice(PokerTournamentBeginSoonNotice);
	notice.Send();
}

void NoticeMgr::NoticeGuildAuctBuyNow(UINT64 guildId, const std::string& roleName, UINT32 price, const std::string& itemName)
{
	Notice notice(GuildAuctBuyNow);
	notice.Replace("$R", roleName);
	notice.Replace("$N", ToString(price));
	notice.Replace("$S", itemName);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildAuctActEnd(UINT64 guildId, const std::string& actName, UINT32 rank)
{
	Notice notice(GuildAuctActEnd);
	notice.Replace("$S", actName);
	notice.Replace("$D", ToString(rank));
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeArenaIntegralReady()
{
	Notice notice(GuildArenaIntegralReady);
	notice.Send();
}

void NoticeMgr::NoticeArenaEnterScene(UINT64 guildId)
{
	Notice notice(GuildArenaEnterScene);
	notice.AddLink();
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeArenaIntegralWin(UINT64 guildId, std::string guildName)
{
	Notice notice(GuildArenaIntegralWin);
	notice.Replace("$G", guildName);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeArenaIntegralLos(UINT64 guildId, std::string guildName)
{
	Notice notice(GuildArenaIntegralLos);
	notice.Replace("$G", guildName);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeIntegralApplyEnd(UINT64 guildId)
{
	Notice notice(GuildIntegralApplyEnd);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildBossRank(std::string guildName, UINT32 rank)
{
	std::string strRank = ToString(rank);
	Notice notice(GuildBossRankNotice);
	notice.Replace("$R", guildName);
	notice.Replace("$N", strRank);
	notice.Send();
}

void NoticeMgr::NoticeGuildAddMember(UINT64 guildId, std::string name)
{
	Notice notice(GuildAddMember);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildMiss(UINT64 guildId)
{
	Notice notice(GuildIntegralMiss);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildJoinLast(UINT64 guildId)
{
	Notice notice(GuildJoinLastBattle);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildEnd(UINT64 roleId)
{
	Notice notice(GuildInheritEnd);
	notice.Replace("$R", ToString(GetGlobalConfig().GuildInheritCon));
	notice.SendRole(roleId);
}

void NoticeMgr::NoticeGuildReq(UINT64 roleId)
{
	Notice notice(GuildInheritReqEnd);
	notice.SendRole(roleId);
}

void NoticeMgr::NoticeGuildRereq(UINT64 roleId)
{
	Notice notice(GuildInheritReqerEnd);
	notice.SendRole(roleId);
}

void NoticeMgr::NoticeNameChange(UINT64 guildId, const std::string& lastname, const std::string& nowname)
{
	Notice notice(ChangeNameNotice2);
	notice.Replace("$C", lastname);
	notice.Replace("$C", nowname);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeProChange(UINT64 guildId, const std::string& name, const std::string& pro)
{
	Notice notice(ChangeProNotice2);
	notice.Replace("$C", name);
	notice.Replace("$C", pro);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticePokerTournamentCardMatch(std::vector<UINT64>& roles, UINT32 type, UINT32 round )
{
	Notice notice(type);
	if (type == PokerTournamentRoundNotice)
	{
		notice.Add(round);
	}
	notice.SendPlays(roles);
}


void NoticeMgr::NoticePokerTournamentFirst(std::vector<UINT64>& roles, CRoleSummary* role)
{
	Notice notice(PokerTournamentFirstNotice);
	notice.AddRole(role);
	notice.SendPlays(roles);
}

void NoticeMgr::NoticeGuildTerrBegin()
{
	Notice notice(GuildTerrBegin);
	notice.Send();
}

void NoticeMgr::NoticeGuildTerrWarning(UINT64 guildId, UINT32 num) 
{
	Notice notice(GuildTerrWarning);
	std::string dest = ToString(num);
	notice.Replace("$R", dest);
	notice.SendGuild(guildId);
} 

void NoticeMgr::NoticeGuildTerrDeclareOne(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrDeclareOne);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrDeclareTwo(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrDeclareTwo);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}
void NoticeMgr::NoticeGuildTerrDeclareThree(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrDeclareThree);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrCanDeclareOne(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrCanDeclareOne);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrCanDeclareTwo(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrCanDeclareTwo);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}
void NoticeMgr::NoticeGuildTerrCanDeclareThree(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrCanDeclareThree);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrWarningDeclare(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrWarningDeclare);
	notice.Replace("$G", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrWarningDeclareTwo(UINT64 guildId, std::string name, std::string nameTwo)
{
	Notice notice(GuildTerrWarningDeclareTwo);
	notice.Replace("$G", name);
	notice.Replace("$R", nameTwo);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrWarningNoDeclare(UINT64 guildId)
{
	Notice notice(GuildTerrWarningNoDeclare);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrNotDeclareOne(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrNotDeclareOne);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}


void NoticeMgr::NoticeGuildTerrNotDeclareTwo(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrNotDeclareTwo);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrNotDeclareThree(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrNotDeclareThree);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrEnterSceneOne(UINT64 guildId)
{
	Notice notice(GuildTerrEnterSceneOne);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrEnterSceneTwo(UINT64 guildId)
{
	Notice notice(GuildTerrEnterSceneTwo);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrEnterSceneThree(UINT64 guildId)
{
	Notice notice(GuildTerrEnterSceneThree);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrEnterScene()
{
	Notice notice(GuildTerrEnterScene);
	notice.Send();
}

void NoticeMgr::NoticeGuildTerrEnd(std::string name)
{
	Notice notice(GuildTerrEnd);
	notice.Replace("$G", name);
	notice.Send();
}

void NoticeMgr::NoticeGuildTerrReward(UINT64 guildId)
{
	Notice notice(GuildTerrReward);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrRewardOne(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrRewardOne);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrRewardTwo(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrRewardTwo);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrRewardThree(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrRewardThree);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrOtherReward(UINT64 guildId)
{
	Notice notice(GuildTerrOtherReward);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrAllianceOne(UINT64 guildId, std::string name, std::string nameTwo)
{
	Notice notice(GuildTerrAllianceOne);
	notice.Replace("$G", name);
	notice.Replace("$R", nameTwo);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrAllianceTwo(UINT64 guildId, std::string name, std::string nameTwo)
{
	Notice notice(GuildTerrAllianceTwo);
	notice.Replace("$G", name);
	notice.Replace("$R", nameTwo);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrAllianceThree(UINT64 guildId, std::string name, std::string nameTwo)
{
	Notice notice(GuildTerrAllianceThree);
	notice.Replace("$G", name);
	notice.Replace("$R", nameTwo);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrDelAlliance(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrDelAlliance);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrDefGuildOne(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrDefGuildOne);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}
void NoticeMgr::NoticeGuildTerrDefGuildTwo(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrDefGuildTwo);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}
void NoticeMgr::NoticeGuildTerrDefGuildThree(UINT64 guildId, std::string name)
{
	Notice notice(GuildTerrDefGuildThree);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);

}

void NoticeMgr::NoticeTeamKickLeaderBefore(UINT64 roleID, UINT32 t)
{
	Notice notice(TeamKickLeaderBefore);
	notice.Replace("$N", ToString(t));
	notice.SendRole(roleID);
}

void NoticeMgr::NoticeTeamKickLeaderAfter(UINT64 roleID)
{
	Notice notice(TeamKickLeaderAfter);
	notice.SendRole(roleID);
}

void NoticeMgr::NoticeGuildBossFirstKill(std::string guildName, std::string bossName)
{
	Notice notice(GuildBossFirstKill);
	notice.Replace("$R", guildName);
	notice.Replace("$N", bossName);
	notice.Send();
}

void NoticeMgr::NoticeLeagueStartApply()
{
	Notice notice(NoticeType_LeagueStartApply);
	notice.Send();
}

void NoticeMgr::NoticeGuildTerrWinOne(UINT64 guildId, std::string nameOne, std::string nameTwo)
{
	Notice notice(NoticeGuildTerrWinBOne);
	notice.Replace("$R", nameOne);
	notice.Replace("$X", nameTwo);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrWinTwo(UINT64 guildId, std::string nameOne, std::string nameTwo)
{
	Notice notice(NoticeGuildTerrWinBTwo);
	notice.Replace("$R", nameOne);
	notice.Replace("$X", nameTwo);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrBwinOne(UINT64 guildId, std::string name)
{
	Notice notice(NoticeGuildTerrBatWinOne);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}
void NoticeMgr::NoticeGuildTerrBwinTwo(UINT64 guildId, std::string name)
{
	Notice notice(NoticeGuildTerrBatWinTwo);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrBwinThree(UINT64 guildId, std::string name)
{
	Notice notice(NoticeGuildTerrBatWinThree);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}

void NoticeMgr::NoticeGuildTerrSavOne(UINT64 guildId, std::string name)
{
	Notice notice(NoticeGuildTerrBatSavOne);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}
void NoticeMgr::NoticeGuildTerrSavTwo(UINT64 guildId, std::string name)
{
	Notice notice(NoticeGuildTerrBatSavTwo);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}
void NoticeMgr::NoticeGuildTerrSavThree(UINT64 guildId, std::string name)
{
	Notice notice(NoticeGuildTerrBatSavThree);
	notice.Replace("$R", name);
	notice.SendGuild(guildId);
}