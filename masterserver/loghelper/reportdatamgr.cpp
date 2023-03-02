#include "pch.h"
#include "reportdatamgr.h"
#include "httptask/reportdata.h"
#include "util/gametime.h"
#include "util.h"
#include "httptask/httptaskmgr.h"
#include "guild/guildmgr.h"
#include "role/rolemanager.h"
#include "guild/guild.h"
#include "social/socialmgr.h"
#include "social/social.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/flowerranklist.h"
#include "guild/guildwage.h"
#include "friend/friendmgr.h"
#include "friend/friend.h"

INSTANCE_SINGLETON(CReportDataMgr)

CReportDataMgr::CReportDataMgr()
{
}

CReportDataMgr::~CReportDataMgr()
{

}
bool CReportDataMgr::Init()
{
	return true;
}

void CReportDataMgr::Uninit()
{

}


void CReportDataMgr::JoinGuild(CRole* role)
{
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	CReportDataTask* poTask = new CReportDataTask(role);
	poTask->AddData(Tx_Reportdata_JoinGuildIDTime, ToString(GameTime::GetTime()));
	poTask->AddData(Tx_Reportdata_GuildMemberChange, "1");
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void CReportDataMgr::GuildLevelUp(CRole* role, UINT32 level)
{
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	CReportDataTask* poTask = new CReportDataTask(role);
	poTask->AddData(Tx_Reportdata_GuildLv, ToString(level));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void CReportDataMgr::CreateGuild(CRole* role, std::string name)
{
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	CReportDataTask* poTask = new CReportDataTask(role);
	poTask->AddData(Tx_Reportdata_GuildCreateTime, ToString(GameTime::GetTime()));
	poTask->AddData(Tx_Reportdata_JoinGuildIDTime, ToString(GameTime::GetTime()));
	poTask->AddData(Tx_Reportdata_GuildMemberChange, "1");
	poTask->AddData(Tx_Reportdata_GuildName, name);
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void CReportDataMgr::DismissGuild(UINT64 roleid)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	CReportDataTask* poTask = new CReportDataTask(role);

	poTask->AddData(Tx_Reportdata_GuildDismissTime, ToString(GameTime::GetTime()));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void CReportDataMgr::LeaveGuild(UINT64 roleid)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	CReportDataTask* poTask = new CReportDataTask(role);
	poTask->AddData(Tx_Reportdata_GuildMemberChange, "2");
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}
void CReportDataMgr::GuildMemberPosChanged(UINT64 roleid, GuildPosition pos)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	INT32 iPos = 0;
	switch (pos)
	{
	case GPOS_LEADER:
		iPos = 1;
		break;
	case GPOS_VICELEADER:
		iPos = 2;
		break;
	default:
		iPos = 3;
		break;
	}
	CReportDataTask* poTask = new CReportDataTask(role);
	poTask->AddData(Tx_Reportdata_GuildPosChange, ToString(iPos));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}
void CReportDataMgr::OnLogin(CRole* role)
{
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	SocialInfo*  info = SocialMgr::Instance()->GetSocialInfo(role->GetID());
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	FlowerRankList* pList = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerTotalRank);
	if (NULL == info && NULL == guild && NULL == pList)
	{
		return;
	}
	CReportDataTask* poTask = new CReportDataTask(role);
	if (info)
	{
		if (info->flower.GetTotalReceiveCount())
		{
			poTask->AddData(Tx_Reportdata_Flower, ToString(info->flower.GetTotalReceiveCount()));
		}
	}
	if (guild)
	{
		if (role->GetID() == guild->GetLeaderID())
		{
			GuildBindInfo& info = guild->GetBindGroupInfo();
			if (info.GetLastUpdateTime() && !info.GetQQGroupCode().empty())
			{
				poTask->AddData(Tx_Reportdata_GuildBindQQ, info.GetQQGroupCode());
			}
		}
		UINT32 rank = CGuildMgr::Instance()->GetRankNum(GuildSortByPrestige, guild->GetID());
		if (rank)
		{
			poTask->AddData(Tx_Reportdata_GuildRank, ToString(rank));
		}
		CGuildWage* pGuildWage = guild->Get<CGuildWage>();
		if (pGuildWage)
		{
			char c = 'D';
			UINT32 wagelv = pGuildWage->GetWageLvl();
			if (wagelv == 1)
			{
				c = 'S';
			}
			else if (wagelv < 6)
			{
				c = 'A' + wagelv - 2;
			}
			poTask->AddData(TX_Reportdata_GuildScoreLv, ToString(c));
		}
	}
	if (pList)
	{
		UINT32 rank = pList->GetRank(role->GetID());
		rank=rank==UINT32(-1)?0:rank;
		if (rank)
		{
			poTask->AddData(Tx_Reportdata_FlowerRank, ToString(rank));
		}
	}
	Friend* pFriend = FriendMgr::Instance()->GetFriendInfo(role->GetID());
	if (pFriend)
	{
		poTask->AddData(Tx_Reportdata_FriendCount, ToString(pFriend->GetFriendCount()));
	}
	poTask->AddData(Tx_Reportdata_CreateTime, ToString(role->GetCreateTime()));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);

}
void CReportDataMgr::GuildBindQQ(UINT64 roleid, UINT32 m_lastUpdateBindTime)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	CReportDataTask* poTask = new CReportDataTask(role);
	poTask->AddData(Tx_Reportdata_GuildBindQQTime, ToString(m_lastUpdateBindTime));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void CReportDataMgr::Common(CRole* role, INT32 type, const string& data)
{
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	CReportDataTask* poTask = new CReportDataTask(role);
	poTask->AddData(type, data);
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void CReportDataMgr::MobaResult(UINT64 roleid, UINT32 todaynum, UINT32 realtotalnum, UINT32 realwinnum, bool iswin)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == role || role->GetLoginType() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}

	CReportDataTask* poTask = new CReportDataTask(role);
	poTask->AddData(Tx_Reportdata_MobaDayCount, ToString(todaynum));
	poTask->AddData(Tx_Reportdata_MobaTotalCount, ToString(realtotalnum));
	if (iswin)
	{
		poTask->AddData(Tx_Reportdata_MobaWinCount, ToString(realwinnum));
	}
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}
