#include "pch.h"
#include "noticemgr.h"
#include "gatelink.h"
#include "unit/role.h"
#include "item.h"
#include "guild/guild.h"
#include "rolesummarymgr.h"
#include "util/timespecificmgr.h"
#include "team.h"
#include "unit/rolemanager.h"
#include "teammgr.h"
#include "team.h"
#include "teammember.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guildchat.h"
#include "guild/guildconfig.h"
#include "gamelogic/rolesummarymgr.h"
#include "foreach.h"
#include "itemconfig.h"
#include "table/globalconfig.h"
#include "scene/sceneteam.h"
#include "scene/scene.h"

INSTANCE_SINGLETON(NoticeMgr);

NoticeMgr::NoticeMgr()
{

}

NoticeMgr::~NoticeMgr()
{

}

bool NoticeMgr::Init()
{
	return LoadFile();
}

void NoticeMgr::Uninit()
{
	ClearFile();
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
	}
	return true;
}

void NoticeMgr::ClearFile()
{
	m_table.Clear();

	m_id2link.clear();
	m_id2paramtypes.clear();
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

UINT32 NoticeMgr::GetLevel(UINT32 index)
{
	NoticeTable::RowData* rowdata = m_table.GetByid(index);
	if (NULL == rowdata)
	{
		return 0;
	}
	return rowdata->level;
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

void NoticeMgr::RawNotice(UINT32 channel, const std::string& info, UINT32 priority)
{
	Notice notice(info, channel, priority);
	notice.Send();
}

//void NoticeMgr::TimeNotice(UINT32 id)
//{
//	Notice notice(id);
//	notice.Send();
//}
//
void NoticeMgr::ItemNotice(Role* role, XItem* item, UINT32 level)
{
	UINT32 noticeid = 0;
	if (EQUIP == item->type)
	{
		if (GetGlobalConfig().EnhanceNotice1.find(level) != GetGlobalConfig().EnhanceNotice1.end())
		{
			noticeid = EnhanceNotice1;
		}
		else if (GetGlobalConfig().EnhanceNotice2.find(level) != GetGlobalConfig().EnhanceNotice2.end())
		{
			noticeid = EnhanceNotice2;
		}
		else if (GetGlobalConfig().EnhanceNotice3.find(level) != GetGlobalConfig().EnhanceNotice3.end())
		{
			noticeid = EnhanceNotice3;
		}
		/*
		if (level >= 9 && level <= 11)
		{
			noticeid = EnhanceNotice1;
		}
		else if (level >= 12 && level <= 14)
		{
			noticeid = EnhanceNotice2;
		}
		else if (level >= 15)
		{
			noticeid = EnhanceNotice3;
		}
		*/
	}
	else if (EMBLEM == item->type && level >= 9)
	{
		noticeid = EmblemNotice;
	}
	else if (JADE == item->type && level >= 9)
	{
		noticeid = JadeNotice;
		Notice notice(noticeid);

		notice.Add(role);
		notice.Add(item);
		notice.Send();

		return;
	}
	if (0 != noticeid)
	{
		Notice notice(noticeid);

		notice.Add(role);
		notice.Add(item);
		notice.Add(level);

		notice.Send();
	}
}

void NoticeMgr::GuildCardNotice(Role* role, UINT64 guildid, UINT32 type, std::vector<ItemDesc>& items)
{
	if (0 == guildid)
	{
		return;
	}
	UINT32 noticeid = 0;
	if (0 == type)
	{
		noticeid = GuildCardNotice1;
	}
	else if (1 == type)
	{
		noticeid = GuildCardNotice2;
	}
	if (0 != noticeid)
	{
		Notice notice(noticeid);
		notice.Add(role);
		if(2 == items.size())
		{
			notice.Add(items[1].itemCount);
			notice.Add(items[0].itemCount);
		}
		notice.SendGuild(guildid);
	}
}

//void NoticeMgr::SendFlowerNotice(Role* srcrole, RoleSummary* destrole, XItem* item)
//{
//	if (!item)
//	{
//		return;
//	}
//	UINT32 itemID = item->itemID;
//	UINT32 count = item->itemCount;
//	FlowerNoticeFunc func = GetSendFlowerNoticeFunc(itemID, count);
//	if (func)
//	{
//		(NoticeMgr::Instance()->*func)(srcrole, destrole, item);
//	}
//}
//
//void NoticeMgr::FlowerNotice(Role* srcrole, RoleSummary* destrole, XItem* item)
//{
//	Notice notice(FlowerNotice1);
//	notice.Add(srcrole);
//	//notice.Add(destrole);
//	notice.Add(item);
//	notice.SendSelf(destrole->GetID());
//}
//
//void NoticeMgr::FlowerNotice2(Role* srcrole, RoleSummary* destrole, XItem* item)
//{
//	Notice notice(FlowerNotice_2);
//	notice.Add(srcrole);
//	notice.Add(destrole);
//	notice.Add(item);
//	notice.Send();
//}
//
//void NoticeMgr::FlowerNotice3(Role* srcrole, RoleSummary* destrole, XItem* item)
//{
//	Notice notice(FlowerNotice_3);
//	notice.Add(srcrole);
//	notice.Add(destrole);
//	notice.Add(item);
//	notice.Add(destrole);
//	notice.Send();
//}
//
//void NoticeMgr::FlowerNotice4(Role* srcrole, RoleSummary* destrole, XItem* item)
//{
//	Notice notice(FlowerNotice_4);
//	notice.Add(srcrole);
//	notice.Add(destrole);
//	notice.Add(item);
//	notice.Add(destrole);
//	notice.Send();
//}
//
//void NoticeMgr::FlowerNotice5(Role* srcrole, RoleSummary* destrole, XItem* item)
//{
//	Notice notice(FlowerNotice_5);
//	notice.Add(srcrole);
//	notice.Add(destrole);
//	notice.Add(item);
//	notice.Add(destrole);
//	notice.Send();
//}

//NoticeMgr::FlowerNoticeFunc NoticeMgr::GetSendFlowerNoticeFunc(UINT32 itemID, UINT32 count)
//{
//	/*const FlowerSendNoticeTable::RowData* data = FlowerConfig::Instance()->GetNoticeData(itemID, count);
//	if (!data)
//	{
//		return NULL;
//	}
//	switch(data->NoticeID)
//	{
//	case FlowerNotice1:
//		return &NoticeMgr::FlowerNotice;
//	case FlowerNotice_2:
//		return &NoticeMgr::FlowerNotice2;
//	case FlowerNotice_3:
//		return &NoticeMgr::FlowerNotice3;
//	case FlowerNotice_4:
//		return &NoticeMgr::FlowerNotice4;
//	case FlowerNotice_5:
//		return &NoticeMgr::FlowerNotice5;
//	default:
//		return NULL;
//	}*/
//	return NULL;
//}

void NoticeMgr::BossNotice(UINT32 type, UINT32 min, UINT64 guildId)
{
	Notice notice(type);
	std::string dest = ToString(min);
	notice.Replace("$N", dest);
	notice.SendGuild(guildId);
}

void NoticeMgr::LotteryNotice(Role* role, XItem* item)
{
	const ItemConf* pConf = ItemConfig::Instance()->GetItemConf(item->itemID);
	if(NULL == pConf) return;
	Notice* notice;
	if(QualityIII == pConf->ItemQuality)
	{
		notice = new Notice(LotteryNoticeA);
	}
	else if(QualityIV == pConf->ItemQuality)
	{
		notice = new Notice(LotteryNoticeS);
	}
	else if(QualityV == pConf->ItemQuality)
	{
		notice = new Notice(LotteryNoticeL);
	}
	else
	{
		return;
	}
	notice->Add(role);
	notice->Add(item);
	RegisterSendDelay(notice, 2);
}

void NoticeMgr::PandoraNotice(Role* role, const ItemDesc& desc, UINT32 quality)
{
	Notice* notice;
	switch (quality)
	{
	case 3:
		notice = new Notice(PandoraNoticeA);
		break;
	case 4:
		notice = new Notice(PandoraNoticeS);
		break;
	case 5:
		notice = new Notice(PandoraNoticeL);
		break;
	default:
		return;
	}
	
	notice->Add(role);
	notice->Add(desc);
	RegisterSendDelay(notice, 2);
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

NoticeLinkInfo* NoticeMgr::GetLinkInfo(UINT32 id)
{
	auto i = m_id2link.find(id);
	if (i == m_id2link.end())
	{
		return NULL;
	}
	return &i->second;
}

void NoticeMgr::RegisterSendDelay(Notice* notice, UINT32 timedelay)
{
	time_t nowtime = time(NULL);
	TimeSpecific time(nowtime + timedelay);

	CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&NoticeMgr::SendDelay, this, notice));
}

bool NoticeMgr::SendDelay(Notice* notice)
{
	notice->Send();
	delete notice;
	return true;
}

//void NoticeMgr::HellHoundNotice( std::string& names )
//{
//	Notice notice(HellHoundHardNotice);
//	notice.Replace("$R", names);
//	notice.Send();
//}
//
//获得鲜花昨日榜称号;
void NoticeMgr::NoticeFlowerDesignation( Role* pRole, UINT32 rank )
{
	SSWarn << "Flower Designation Notice, RoleID:" << pRole->GetID() << ", Rank:" << rank << END;
	std::string strRank = ToString(rank);
	Notice notice(FlowerDesignationNotice);
	notice.Replace("$R", pRole->GetName());
	notice.Replace("$I", strRank);
	notice.SendSelf(pRole);
	//notice.SendSelf(pRole);
}

void NoticeMgr::NoticeGuildArenaKickYou(UINT64 roleID)
{
	Notice notice(GuildArenaKickYou);
	notice.SendSelf(roleID);
}

void NoticeMgr::NoticeGuildBossLastKill(std::string roleName, XItem* item, UINT64 guildId)
{
	Notice notice(GuildBossLastKill);
	notice.Replace("$R", roleName);
	notice.Add(item);
	notice.SendGuild(guildId);
}

///>公会可以开启答题提示	
void NoticeMgr::GuildQALeftTimeNotice( UINT64 guildID, UINT32 leftMin )
{
	Notice notice(GuildQALeftTime_Arrive);
	notice.SendGuild(guildID);
}

/////>公会官员开启了公会答题通知
//void NoticeMgr::GuildLeaderOpenQANotice( UINT64 guildID )		
//{
//	Notice notice(GuildQALeftTime_QAOpen);
//	notice.SendGuild(guildID);
//}

/////>参与全民答题倒计时提示
//void NoticeMgr::JoinWorldQANotice( UINT32 min )
//{
//	std::string strMin = ToString(min);
//	Notice notice(TakeWorldQANotice);
//	notice.Replace("$R", strMin);
//	notice.Send();
//	///>SSInfo << "Send Join World QA Notice, Minute:" << min << END;
//}
//
/////>参与公会答题倒计时提示
//void NoticeMgr::JoinGuildQANotice( UINT64 guildID, UINT32 min)
//{
//	std::string strMin = ToString(min);
//	Notice notice(TakeGuildQANotice);
//	notice.Replace("$R", strMin);
//	notice.SendGuild(guildID);
//	///>SSInfo << "Send Join Guild QA Notice, Minute:" << min << END;
//}

void NoticeMgr::NoticeFetchRoll(Role* pRole, XItem* item, UINT32 result)
{
	if (!pRole)
	{
		return;
	}
	if (!pRole->GetCurrScene() || !pRole->GetCurrScene()->GetSceneTeam())
	{
		return;
	}
	int teamid = pRole->GetCurrScene()->GetSceneTeam()->GetTeamID();
	if(0 == teamid)
	{
		return;
	}

	Notice selfNotice(RollResultTeamNotice);
	selfNotice.Add(pRole);
	selfNotice.Add(item);
	selfNotice.Add(result);
	const auto& roles = pRole->GetCurrScene()->GetAllRoles();
	for (auto it = roles.begin(); it != roles.end(); ++it)
	{
		selfNotice.SendSelf(*it);
	}
}

void NoticeMgr::NoticeGiveUpFetchRoll(Role* pRole, XItem* item)
{
	if (!pRole)
	{
		return;
	}
	if (!pRole->GetCurrScene() || !pRole->GetCurrScene()->GetSceneTeam())
	{
		return;
	}
	int teamid = pRole->GetCurrScene()->GetSceneTeam()->GetTeamID();
	if(0 == teamid)
	{
		return;
	}

	Notice selfNotice(RollGiveUpSelfNotice);
	selfNotice.Add(item);
	selfNotice.SendSelf(pRole);

	Notice teamNotice(RollGiveUpTeamNotice);
	teamNotice.Add(pRole);
	teamNotice.Add(item);
	const auto& roles = pRole->GetCurrScene()->GetAllRoles();
	for (auto it = roles.begin(); it != roles.end(); ++it)
	{
		teamNotice.SendSelf(*it);
	}
}

void NoticeMgr::NoticeAllFetchRoll(Role* pRole, XItem* item, UINT32 max)
{
	if (!pRole)
	{
		return;
	}
	if (!pRole->GetCurrScene() || !pRole->GetCurrScene()->GetSceneTeam())
	{
		return;
	}
	int teamid = pRole->GetCurrScene()->GetSceneTeam()->GetTeamID();
	if(0 == teamid)
	{
		return;
	}

	Notice selfNotice(RollFinalResultSelfNotice);
	selfNotice.Add(max);
	selfNotice.Add(item);
	selfNotice.SendSelf(pRole);

	Notice teamNotice(RollFinalResultTeamNotice);
	teamNotice.Add(pRole);
	teamNotice.Add(max);
	teamNotice.Add(item);
	const auto& roles = pRole->GetCurrScene()->GetAllRoles();
	for (auto it = roles.begin(); it != roles.end(); ++it)
	{
		teamNotice.SendSelf(*it);
	}
}

void NoticeMgr::NoticeSendNestLiveWatch(Role* pRole, const std::string& names, int num)
{
	Notice notice(LiveNotice_Nest);
	notice.Add(pRole);
	notice.Replace("$C", names);
	notice.Add(num);
	notice.AddLink();
	if (GSConfig::Instance()->IsCrossGS())
	{
		;//暂不支持
	}
	else
	{
		notice.Send();
	}
}

void NoticeMgr::NoticeSendDragonLiveWatch(Role* pRole, const std::string& names)
{
	Notice notice(LiveNotice_Dragon);
	notice.Add(pRole);
	notice.Replace("$C", names);
	notice.AddLink();
	if (GSConfig::Instance()->IsCrossGS())
	{
		;//暂不支持
	}
	else
	{
		notice.Send();
	}
}

void NoticeMgr::NoticeSend1V1LiveWatch(Role* pRole1, Role* pRole2)
{
	Notice notice(LiveNotice_1V1);
	notice.Add(pRole1);
	notice.Add(pRole2);
	notice.AddLink();
	if (GSConfig::Instance()->IsCrossGS())
	{
		;//暂不支持
	}
	else
	{
		notice.Send();
	}
}

void NoticeMgr::NoticeSend2V2LiveWatch(Role* pRole1, Role* pRole2)
{
	Notice notice(LiveNotice_2V2);
	notice.Add(pRole1);
	notice.Add(pRole2);
	notice.AddLink();
	if (GSConfig::Instance()->IsCrossGS())
	{
		;//暂不支持
	}
	else
	{
		notice.Send();
	}
}

void NoticeMgr::NoticeSendProtectCaptainLiveWatch(Role* pRole1, Role* pRole2)
{
	Notice notice(LiveNotice_ProtectCaptain);
	notice.Add(pRole1);
	notice.Add(pRole2);
	notice.AddLink();
	if (GSConfig::Instance()->IsCrossGS())
	{
		;//暂不支持
	}
	else
	{
		notice.Send();
	}
}

void NoticeMgr::NoticeSendGuildBattleLiveWatch(UINT64 guildID1, UINT64 guildID2)
{
	Notice notice(LiveNotice_GuildBattle);
	notice.AddGuild(guildID1);
	notice.AddGuild(guildID2);
	notice.AddLink();
	notice.Send();
}

void NoticeMgr::NoticeSendHeroBattleLiveWatch(Role* role1, Role* role2, UINT32 num)
{
	Notice notice(LiveNotice_HeroBattle);
	notice.Add(role1);
	notice.Add(role2);
	notice.Add(num);
	notice.AddLink();
	if (GSConfig::Instance()->IsCrossGS())
	{
		;//暂时不支持
	}
	else
	{
		notice.Send();
	}
}

void NoticeMgr::NoticeSendLeagueBattleLiveWatch(std::string& name1, std::string& name2, UINT32 num)
{
	Notice notice(LiveNotice_LeagueBattle);
	notice.Replace("$C", name1);
	notice.Replace("$C", name2);
	notice.Add(num);
	notice.AddLink();
	if (GSConfig::Instance()->IsCrossGS())
	{
		;//暂不支持
	}
	else
	{
		notice.Send();
	}
}

void NoticeMgr::NoticeSendCustomPkLiveWatch(Role* pRole1, Role* pRole2)
{
	Notice notice(LiveNotice_CustomPk);
	notice.Add(pRole1);
	notice.Add(pRole2);
	notice.AddLink();
	if (GSConfig::Instance()->IsCrossGS())
	{
		;//暂不支持
	}
	else
	{
		notice.Send();
	}
}

void NoticeMgr::NoticeGuildArenaKick(Role* pRole, Role* kicker)
{
	if(NULL == pRole || NULL == kicker)
	{
		return ;
	}
	Notice notice(GuildArenaKickNotice);
	notice.Replace("$R", pRole->GetName());
	notice.SendSelf(pRole);
}

void NoticeMgr::NoticeGuildArenaOneFightEnd(Role* winRole, Role* loseRole, UINT64 winGuildID, UINT64 loseGuildID)
{
	if(NULL == winRole || NULL == loseRole)
	{
		return ;
	}

	{
		Notice notice(GuildArenaOneFightWin);
		notice.Replace("$R", winRole->GetName());
		notice.Replace("$R", loseRole->GetName());
		notice.SendGuild(winGuildID);
	}

	{
		Notice notice(GuildArenaOneFightLose);
		notice.Replace("$R", loseRole->GetName());
		notice.Replace("$R", winRole->GetName());
		notice.SendGuild(loseGuildID);
	}

}

//void NoticeMgr::NoticeGuildBossFirstKill(std::string guildName, std::string bossName)
//{
//	Notice notice(GuildBossFirstKill);
//	notice.Replace("$R", guildName);
//	notice.Replace("$N", bossName);
//	notice.Send();
//}


void NoticeMgr::NoticeGuildEnd(UINT64 roleId)
{
	Notice notice(GuildInheritEnd);
	notice.Replace("$R", ToString(GetGlobalConfig().GuildInheritCon));
	notice.SendSelf(roleId);
}

void NoticeMgr::NoticeGuildReq(UINT64 roleId)
{
	Notice notice(GuildInheritReqEnd);
	notice.SendSelf(roleId);
}

void NoticeMgr::NoticeGuildRereq(UINT64 roleId)
{
	Notice notice(GuildInheritReqerEnd);
	notice.SendSelf(roleId);
}

///>公会营地	
void NoticeMgr::GuildCampPartyLeftTimeNotice( UINT64 guildID, UINT32 leftMin )
{
	Notice notice(GuildQALeftTime_Arrive);
	notice.SendGuild(guildID);
}

void NoticeMgr::NoticeBioHellFirstComplete(UINT64 roleId, UINT32 sceneid)
{
	auto sceneConf = SceneConfig::Instance()->FindSceneInfo(sceneid);
	if (!sceneConf || !sceneConf->m_pConf)
	{
		return;
	}
	Notice notice(BioHellFirstComplete);
	notice.Replace("$C", sceneConf->m_pConf->Comment);
	notice.SendSelf(roleId);
}
