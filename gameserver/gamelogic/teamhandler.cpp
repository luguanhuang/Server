#include "pch.h"
#include "teamhandler.h"
#include "scene/scene.h"
#include "globalconfig.h"
#include "team.h"
#include "table/expeditionconfigmgr.h"
#include "stagemgr.h"
#include "unit/role.h"
#include "guild/guildmgr.h"
#include "foreach.h"
#include "teammember.h"
#include "guild/guild.h"
#include "team.h"
#include "scene/enterscenecondition.h"
#include "unit/rolemanager.h"
#include "bagtransition.h"
#include "gamelogic/dragonrecord.h"
#include "gamelogic/towerconfig.h"
#include "towerrecord.h"
#include "guild/guilddef.h"
#include "idip/idiprecord.h"
#include "define/systemiddef.h"
#include "teamrecord.h"
#include "gamelogic/rolepartner.h"
#include "holidaystage.h"
#include "gamelogic/levelsealMgr.h"
#include "matchhandler.h"
#include "spactivity/spactivitydatabase.h"
#include "gamelogic/competedragonrecord.h"
#include "gamelogic/dragonconfig.h"

std::unordered_map<int, ITeamTypeHandlerBase*> ITeamTypeHandlerBase::m_TypeHandler;

ExpTeamHandler ExpTeamHandler::GlobalExpTeamHandler;
GuildTeamHandler GuildTeamHandler::GlobalGuildTeamHandler;
NestTeamHandler NestTeamHandler::GlobalNextTeamHandler;
AbyssTeamHandler AbyssTeamHandler::GlobalAbyssTeamHandler;
DragonTeamHandler DragonTeamHandler::GlobalDragonTeamHandler;
GoddessTeamHandler GoddessTeamHandler::GlobalGoddessTeamHandler;
PvpTeamHandler PvpTeamHandler::GlobalPvpTeamHandler;
PkTwoTeamHandler PkTwoTeamHandler::GlobalPvpTeamHandler;
EndlessabyssTeamHandler EndlessabyssTeamHandler::GlobalEndlessabyssTeamHandler;
SkyCityTeamHandler SkyCityTeamHandler::GlobalSkyCityTeamHandler;
ResWarTeamHandler  ResWarTeamHandler::GlobalResWarTeamHandler;
GuildCampTeamHandler  GuildCampTeamHandler::GlobalGuildCampTeamHandler;
WeekNestTeamHandler WeekNestTeamHandler::GlobalWeekNestTeamHandler;
PartnerTeamHandler PartnerTeamHandler::GlobalPartnerTeamHandler;
//TowerTeamHandler TowerTeamHandler::GlobalTowerTeamHandler;
HeroBattleTeamHandler HeroBattleTeamHandler::GlobalHeroBattleTeamHandler;
MobaBattleTeamHandler MobaBattleTeamHandler::GlobalMobaBattleTeamHandler;
LeagueTeamHandler LeagueTeamHandler::GlobalLeagueTeamHandler;
SkyCraftTeamHandler SkyCraftTeamHandler::GlobalSkyCraftTeamHandler;
FestivalTeamHandler FestivalTeamHandler::GlobalFestivalTeamHandler;
TaskTeamHandler TaskTeamHandler::GlobalTaskTeamHandler;
WeekEnd4v4TeamHandler WeekEnd4v4TeamHandler::GlobalWeekEnd4v4TeamHandler;
CustomBattlePkTwoTeamHandler CustomBattlePkTwoTeamHandler::GlobalCustomBattlePkTwoTeamHandler;
BioHellTeamHandler BioHellTeamHandler::GlobalBioHellTeamHandler;
MarriageCoupleTeamHandler MarriageCoupleTeamHandler::GlobalMarriageCoupleTeamHandler;
MarriageTeamHandler MarriageTeamHandler::GlobalMarriageTeamHandler;
CompeteDragonTeamHandler CompeteDragonTeamHandler::GlobalCompeteDragonTeamHandler;

UINT32 GetSampleSceneID( ExpeditionTable::RowData *pConf )
{
	if(NULL == pConf)
	{
		return 0;
	}
	return ExpeditionConfigMgr::Instance()->GetExpRandomScene(pConf->DNExpeditionID);
}

ITeamTypeHandlerBase* ITeamTypeHandlerBase::GetTeamHanderByType( int pType )
{
	auto it = m_TypeHandler.find(pType);
	if(it != m_TypeHandler.end())
	{
		return (it->second);
	}
	return NULL;
}

int ITeamTypeHandlerBase::ChangeExpIDBy(TeamCopy * pTeam , int expID , UINT64& problemID)
{
	problemID = 0;

	if(NULL == pTeam)
	{
		return KKSG::ERR_FAILED;
	}

	ExpeditionTable::RowData *pRow = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
	if (pRow == NULL)
	{
		return KKSG::ERR_TEAM_EXPEDITIONID_NOT_EXIST;
	}

	if(pRow->PlayerNumber < (int)pTeam->GetMemberCount())
	{
		return KKSG::ERR_TEAM_MEMCOUNT_OVER;
	}

	ITeamTypeHandlerBase *nextHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pRow->Type);
	if(NULL == nextHandler)
	{
		return KKSG::ERR_FAILED;
	}

	int errcode = CheckRoleInGuild(pRow, pTeam->GetLeaderID(), pTeam->GetMember(), problemID);
	if(KKSG::ERR_SUCCESS != errcode)
	{
		return errcode;
	}
	
	auto& pMembers = pTeam->GetMember();

	foreach(i in pMembers)
	{
		if ((*i).IsRobot())
			continue;
		problemID = (*i).RoleID(); 
		Role* memRole = RoleManager::Instance()->FindByRoleID((*i).RoleID());
		if(NULL == memRole || memRole->IsInBattle())
		{
			return KKSG::ERR_TEAM_NOT_IN_HALL;
		}
		int ret = nextHandler->CheckOpenCondition(memRole, pRow);
		if(0 != ret)
		{
			return ret;
		}
	}
	return KKSG::ERR_SUCCESS;
}

int ITeamTypeHandlerBase::CheckBattleStartCondition(TeamCopy *pTeam, UINT64& problemID)
{
	problemID = 0;

	if(NULL == pTeam)
	{
		return KKSG::ERR_FAILED;
	}

	ExpeditionTable::RowData * pConf = pTeam->GetConf();	

	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	int MemberCount = pTeam->GetMemberCount();
	if (MemberCount < pConf->PlayerLeastNumber)
	{
		return KKSG::ERR_TEAM_NEED_ATLEAST_2_MEMBER;
	}
	if (MemberCount > pConf->PlayerNumber)
	{
		return KKSG::ERR_TEAM_MEMCOUNT_OVER;
	}
	if (pTeam->IsTeamCost())
	{
		if (MemberCount != pConf->PlayerNumber)
		{
			return KKSG::ERR_TEAMCOST_NUMLIMIT;
		}
	}

	//目前只有teamtype=2 使用 且没有开放
	if (!pConf->OpenTime.empty())
	{
		bool isOpen = false;
		time_t dwNowTime = time(NULL);
		tm* stTime = localtime(&dwNowTime);
		int now = stTime->tm_hour * 3600 + stTime->tm_min * 60 + stTime->tm_sec;
		for (auto i = pConf->OpenTime.begin(); i != pConf->OpenTime.end(); ++i)
		{
			if (i->seq[0] <= now && now <= i->seq[1])
			{
				isOpen = true;
				break;
			}
		}

		if (isOpen == false)
		{
			return KKSG::ERR_TEAM_NOT_OPENTIME;
		}
	}

	auto& pMembers = pTeam->GetMember();

	for(size_t i = 0; i < pMembers.size(); ++i)
	{
		const TeamCopyMember& member = pMembers[i];
		if (member.IsRobot())
			continue;

		problemID = member.RoleID();

		Role *pRole = RoleManager::Instance()->FindByRoleID(member.RoleID());
		if (NULL == pRole)
		{
			return KKSG::ERR_TEAM_NOT_IN_HALL;
		}

		if (NULL == pRole->GetCurrScene())
		{
			return KKSG::ERR_TEAM_NOT_IN_HALL;
		}

		if(pRole->IsInBattle())
		{
			return KKSG::ERR_TEAM_NOT_IN_HALL;
		}

		TeamEnterData tmp(member.GetMemberType());
		int ret = CheckEnterCondtion(pRole, pConf, &tmp);
		TJUDGERET(ret)

	}

	int errcode = CheckRoleInGuild(pConf, pTeam->GetLeaderID(), pMembers, problemID);
	if(KKSG::ERR_SUCCESS != errcode)
	{
		return errcode;
	}

	return KKSG::ERR_SUCCESS;
}

int ITeamTypeHandlerBase::CheckRoleInGuild(ExpeditionTable::RowData *pConf, UINT64 leaderID, const std::vector<TeamCopyMember> &members, UINT64& problemRoleID)
{
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}
	if(0 == pConf->GuildLevel)//不需要同公会
	{
		return KKSG::ERR_SUCCESS;
	}
	UINT64 qwGuildID = GuildSimpleMgr::Instance()->GetGuildId(leaderID);
	if (qwGuildID == INVALID_GUILDID)
	{
		problemRoleID = leaderID;
		return KKSG::ERR_GUILD_NOT_IN_GUILD;
	}

	foreach(i in members)
	{
		if((*i).IsRobot())
		{
			continue;
		}
		UINT64 qwRole = (*i).RoleID();

		problemRoleID = qwRole;

		if (qwGuildID != GuildSimpleMgr::Instance()->GetGuildId(qwRole))
		{
			return KKSG::ERR_GUILD_NOT_IN_GUILD;
		}
	}
	return KKSG::ERR_SUCCESS;
}

int ITeamTypeHandlerBase::CheckIdipCondition(Role* pRole)
{
	if(pRole->Get<CIdipRecord>()->CheckIdipAndNotify(KKSG::PUNISH_USER_DAILY_PLAY, false))
	{
		return KKSG::ERR_TEAM_IDIP;
	}
	return KKSG::ERR_SUCCESS;
}

ITeamTypeHandlerBase::ITeamTypeHandlerBase(int type)
{
	m_TypeHandler[type] = this;
	m_type = type;
}

int ITeamTypeHandlerBase::GetDayCountLeft(Role *pRole)
{
	int leftCount = 0;
	leftCount = pRole->Get<CTeamRecord>()->GetTeamLeftCountToday(m_type);
	return (leftCount < 0) ? 0 : leftCount;
}

//一些基本判断
int ITeamTypeHandlerBase::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	if (pRole->GetLevel() < pRow->RequiredLevel)
	{
		return KKSG::ERR_SCENE_LEVELREQ;
	}

	if(pRow->GuildLevel > 0)//需要判断公会等级
	{
		CGuildSimple *pGuild = GuildSimpleMgr::Instance()->GetGuildByRoleId(pRole->GetID());
		if (pGuild == NULL)
		{
			return KKSG::ERR_GUILD_NOT_IN_GUILD;
		}

		if ((int)pGuild->GetGuildLvl() < pRow->GuildLevel)
		{
			return KKSG::ERR_GUILD_LEVEL_REQ;
		}
	}

	//检测前置场景  只适用于单场景关卡
	{
		SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(GetSampleSceneID(pRow));
		if(NULL == pSceneInfo || NULL == pSceneInfo->m_pConf)
		{
			return KKSG::ERR_SCENE_NOT_IN_CONFIG;
		}
		if (!SceneConfig::Instance()->CheckPreScene(pRole, pSceneInfo->m_pConf->PreScene))
		{
			return KKSG::ERR_SCENE_NEED_PRESCENE;
		}
	}

	int ret = KKSG::ERR_SUCCESS;
	//和匹配相关
	if(KKSG::KMT_NONE != pRow->MatchType)
	{
		MatchHandler handler;
		ret = handler.RoleCondition(pRole, pRow->MatchType);
		TJUDGERET(ret)
	}

	ret = CheckIdipCondition(pRole);
	TJUDGERET(ret)

	return KKSG::ERR_SUCCESS;
}

int ITeamTypeHandlerBase::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
	int ret = CheckOpenCondition(pRole, pRow);
	TJUDGERET(ret)

	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(GetSampleSceneID(pRow));
	if(NULL == pSceneInfo)
	{
		return KKSG::ERR_SCENE_NOT_IN_CONFIG;
	}

	if(pExtrData->IsHelper())
	{
		ret = CheckHelperCondition(pRole, pRow);	
		return ret;
	}

	if(pExtrData->IsUseTicket())
	{
		ret = CheckUseTicketCondition(pRole, pRow);		
		TJUDGERET(ret)
	}
	else
	{
		if (TEAM_CCT_NONE != pRow->CostCountType && GetDayCountLeft(pRole) == 0)
		{
			return KKSG::ERR_SCENE_TODYCOUNTLIMIT;
		}		
		EnterSceneFatigueCondition cond;
		ret = cond.CanEnterScene(pRole, pSceneInfo);
		TJUDGERET(ret)
	}

	//星级进度
	//auto pmgr = ExpeditionConfigMgr::Instance();
	//if(pmgr->IsStarExp(pRow))
	//{
	//	if(pRole->Get<CTeamRecord>()->GetStarProgressByType(pmgr->GetStarExpType(pRow)) != pRow->DNExpeditionID)
	//	{
	//		return KKSG::ERR_DRAGON_PROGRESS_INVALID;//复用海龙的
	//	}
	//}

	return KKSG::ERR_SUCCESS;
}

int ITeamTypeHandlerBase::CheckHelperCondition(Role* pRole, ExpeditionTable::RowData *pRow)
{
	if(0 == pRow->CanHelp)//不可以助战
	{
		return KKSG::ERR_FAILED;
	}
	else if(1 == pRow->CanHelp)//需要通关才可以助战
	{
		UINT32 sceneTempID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(pRow->DNExpeditionID);
		bool isHardDragon = false;
		auto data = DragonConfig::Instance()->GetDragonTableData(pRow->DNExpeditionID);
		if (data && data->DragonNestDifficulty == 2)
		{
			isHardDragon = true;
		}

		if((pRow->Type == TEAM_TYPE_COMPETE_DRAGON || isHardDragon) && !pRow->RandomSceneIDs.empty())
		{
			sceneTempID = ExpeditionConfigMgr::Instance()->GetRandomScene(pRow->RandomSceneIDs.back());
		}

		if(0 == sceneTempID)
		{
			return KKSG::ERR_FAILED;
		}

		if(!pRole->Get<StageMgr>()->IsPassed(sceneTempID))
		{
			return KKSG::ERR_TEAM_NOT_PASS;
		}
	}

	return KKSG::ERR_SUCCESS;
}

int ITeamTypeHandlerBase::CheckUseTicketCondition(Role* pRole, ExpeditionTable::RowData *pRow)
{
	if(0 == pRow->UseTicket[0])
	{
		return KKSG::ERR_TEAM_TICKET_CONFIG;
	}

	if(CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType < pRow->UseTicket[1])
	{
		return KKSG::ERR_TEAM_TICKET_SEAL;
	}

	if(pRole->Get<CTeamRecord>()->GetUseTicketLeftCount() <= 0)
	{
		return KKSG::ERR_TEAM_USE_TICKET_COUNT_LESS;
	}

	if(pRole->Get<Bag>()->CountItemInBag(pRow->UseTicket[0]) <= 0)
	{
		return KKSG::ERR_TEAM_TICKET_LESS;
	}

	return KKSG::ERR_SUCCESS;
}


int NestTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}
	
	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_TEAM_NEST))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	return KKSG::ERR_SUCCESS;
}

void NestTeamHandler::BuyCount( Role* pRole, KKSG::BuyTeamSceneCountRet& roRes )
{
	if(NULL == pRole)
	{
		roRes.set_errcode(KKSG::ERR_UNKNOWN);
		return ;
	}
	Bag* bag = pRole->Get<Bag>();
	CTeamRecord* stage = pRole->Get<CTeamRecord>();
	int count = stage->GetTeamBuyCountToday(NEST_TEAM_SCENE);
	if(count >=  GetGlobalConfig().GetInt("BuyNestCountLimit", 10))
	{
		roRes.set_errcode(KKSG::ERR_TEAMBUY_COUNT_MAX);
		return ;
	}
	std::vector<UINT32>& cost =  GetGlobalConfig().BuyNestCountCost;
	if(cost.empty())
	{
		roRes.set_errcode(KKSG::ERR_UNKNOWN);
		return ;
	}
	size_t index = (size_t)count;
	if(index >= cost.size())
	{
		index = cost.size() - 1;
	}
	UINT32 need = cost[index];
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Stage, ItemFlow_NestBuy);
	if(take.TakeItem(DRAGON_COIN, need))
	{
		stage->AddTeamExtraAddCount(NEST_TEAM_SCENE);
		stage->AddTeamBuyCountToday(NEST_TEAM_SCENE);

		int maxcount = stage->GetTeamMaxCountToday(NEST_TEAM_SCENE);
		int buycount = stage->GetTeamBuyCountToday(NEST_TEAM_SCENE);
		int entercount = stage->GetTeamFinishCountToday(NEST_TEAM_SCENE);
		roRes.set_maxcount(maxcount);
		roRes.set_buycount(buycount);
		roRes.set_entercount(entercount);
	}
	else
	{
		take.RollBack();
		roRes.set_errcode(KKSG::ERR_TEAMBUY_DRAGONCOIN_LESS);
		return ;
	}
	take.NotifyClient();
	roRes.set_errcode(KKSG::ERR_SUCCESS);
}

void AbyssTeamHandler::BuyCount( Role* pRole, KKSG::BuyTeamSceneCountRet& roRes )
{
	if(NULL == pRole)
	{
		roRes.set_errcode(KKSG::ERR_UNKNOWN);
		return ;
	}
	Bag* bay = pRole->Get<Bag>();
	CTeamRecord* stage = pRole->Get<CTeamRecord>();
	int count = stage->GetTeamBuyCountToday(ABYSS_TEAM_SCENE);
	if(count >=  GetGlobalConfig().GetInt("BuyAbyssCountLimit", 10))
	{
		roRes.set_errcode(KKSG::ERR_TEAMBUY_COUNT_MAX);
		return ;
	}
	std::vector<UINT32>& cost =  GetGlobalConfig().BuyAbyssCountCost;
	if(cost.empty())
	{
		roRes.set_errcode(KKSG::ERR_UNKNOWN);
		return ;
	}
	size_t index = (size_t)count;
	if(index >= cost.size())
	{
		index = cost.size() - 1;
	}
	UINT32 need = cost[index];
	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Stage, ItemFlow_AbysssBuy);
	if(take.TakeItem(DRAGON_COIN, need))
	{
		stage->AddTeamBuyCountToday(ABYSS_TEAM_SCENE);
		stage->AddTeamExtraAddCount(ABYSS_TEAM_SCENE);

		int maxcount = stage->GetTeamMaxCountToday(ABYSS_TEAM_SCENE);
		int buycount = stage->GetTeamBuyCountToday(ABYSS_TEAM_SCENE);
		int entercount = stage->GetTeamFinishCountToday(ABYSS_TEAM_SCENE);
		roRes.set_maxcount(maxcount);
		roRes.set_buycount(buycount);
		roRes.set_entercount(entercount);
	}
	else
	{
		take.RollBack();
		roRes.set_errcode(KKSG::ERR_TEAMBUY_DRAGONCOIN_LESS);
		return ;
	}
	take.NotifyClient();
	roRes.set_errcode(KKSG::ERR_SUCCESS);
}

int DragonTeamHandler::CheckEnterCondtion( Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
	int ret = ITeamTypeHandlerBase::CheckEnterCondtion(pRole, pRow, pExtrData);
	TJUDGERET(ret)

	//useTicket 龙本如果要求可以使用useTicket 则需要策划定义行为  

	if(pExtrData->IsHelper())
	{
		return KKSG::ERR_SUCCESS;
	}

	CDragonRecord* record = pRole->Get<CDragonRecord>();
	if(!record->CheckRoleCondition(pRow->DNExpeditionID))
	{
		return KKSG::ERR_DRAGON_PROGRESS_INVALID;
	}

	if(pExtrData->IsGotoNext())
	{
		return KKSG::ERR_SUCCESS;
	}

	Bag* bag = pRole->Get<Bag>();
	foreach(i in pRow->CostItem)
	{
		if(XItem::IsVirtualItem(i->seq[0]) && bag->CountVirtualItem(i->seq[0]) < i->seq[1])
			return KKSG::ERR_DRAGON_TICKET_NOTENOUGH;
		if(!XItem::IsVirtualItem(i->seq[0]) && (int)bag->CountItemInBag(i->seq[0]) < i->seq[1])
			return KKSG::ERR_DRAGON_TICKET_NOTENOUGH;
	}

	return KKSG::ERR_SUCCESS;
}

int SkyCityTeamHandler::CheckIdipCondition(Role* pRole)
{
	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		return KKSG::ERR_FAILED;
	}
	return KKSG::ERR_SUCCESS;
}

int ResWarTeamHandler::CheckIdipCondition(Role* pRole)
{
	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		return KKSG::ERR_FAILED;
	}
	return KKSG::ERR_SUCCESS;
}

//int TowerTeamHandler::GetDayCountLeft( Role *pRole )
//{
//	int leftCount = 0;
//	StageMgr *pStage = pRole->Get<StageMgr>();
//	leftCount = pStage->GetTeamLeftCountToday(TEAM_TOWER);
//	return (leftCount < 0) ? 0 : leftCount;
//}
//
//int TowerTeamHandler::GetDayCountLeft( Role *pRole, ExpeditionTable::RowData *pConf )
//{
//	return GetDayCountLeft(pRole);
//}
//
//int TowerTeamHandler::CheckEnterCondtion( Role *pRole, ExpeditionTable::RowData *pRow, UINT64 *pGID )
//{
//	int ret = CheckOpenCondition(pRole, pRow, pGID);
//	if(KKSG::ERR_SUCCESS != ret)
//	{
//		return ret;
//	}
//
//	if (GetDayCountLeft(pRole, pRow) == 0)
//	{
//		return KKSG::ERR_SCENE_TODYCOUNTLIMIT;
//	}
//
//	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(GetSampleSceneID(pRow));
//	if(NULL == pSceneInfo)
//	{
//		return KKSG::ERR_SCENE_NOT_IN_CONFIG;
//	}
//
//	EnterSceneFatigueCondition cond;
//	ret = cond.CanEnterScene(pRole, pSceneInfo);
//	if (KKSG::ERR_SUCCESS != ret)
//	{
//		return ret;
//	}
//
//	return KKSG::ERR_SUCCESS;
//}
//
//int TowerTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow, UINT64 *pGID)
//{
//	if (pRole->GetLevel() < pRow->RequiredLevel)
//	{
//		return KKSG::ERR_SCENE_LEVELREQ;
//	}
//
//	if (pRow->RandomSceneIDs.empty())
//	{
//		return KKSG::ERR_FAILED;
//	}
//
//	int floorSize = pRow->RandomSceneIDs.size();
//
//	/*int sceneID = ExpeditionConfigMgr::Instance()->GetRandomScene(pRow->RandomSceneIDs[0]);
//	auto sceneids =  GetGlobalConfig().TeamTowerSceneID;
//	int hardLevel = 0;
//	foreach(i in sceneids)
//	{
//		if ((*i).seq[1] == sceneID)
//		{
//			hardLevel = (*i).seq[0];
//			break;
//		}
//	}
//	if(!hardLevel)
//		return KKSG::ERR_FAILED;*/
//	int hardLevel = TowerConfig::Instance()->GetHardLevel(pRow);
//	if(!hardLevel)
//		return KKSG::ERR_FAILED;
//
//	CTowerRecord* record = pRole->Get<CTowerRecord>();
//	if (!record)
//	{
//		return KKSG::ERR_FAILED;
//	}
//	/*int openLevel = record->GetTowerInfo().openHardLevel;
//	int reachFloor = record->GetTowerInfo().reachTopFloor;
//	if (openLevel && hardLevel > openLevel && !(reachFloor == floorSize && (hardLevel - openLevel == 1)))
//	{
//		return KKSG::ERR_FAILED;
//	}*/
//
//	/*if (!record->CheckEnterCondtion(hardLevel, floorSize))
//	{
//		return KKSG::ERR_FAILED;
//	}*/
//	return record->CheckRoleCondition(hardLevel, floorSize);
//}


int WeekNestTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_WEEK_NEST_ID))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	return KKSG::ERR_SUCCESS;
}

int PartnerTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	RolePartner* p = pRole->Get<RolePartner>();	
	return p->CheckCanMakePartner();
}

int LeagueTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}
	// TODO?
	return KKSG::ERR_SUCCESS;
}

int SkyCraftTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	if (KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}

int FestivalTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	TJUDGERET(ret)

	if(!CHolidayStage::Instance()->IsHolidaySecneId(GetSampleSceneID(pRow)))
	{
		return KKSG::ERR_FAILED;// TODO
	}
	return KKSG::ERR_SUCCESS;
}

int FestivalTeamHandler::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
	int ret = ITeamTypeHandlerBase::CheckEnterCondtion(pRole, pRow, pExtrData);
	TJUDGERET(ret)

	if(pExtrData->IsHelper())
	{
		return KKSG::ERR_SUCCESS;
	}

	if(CHolidayStage::Instance()->GetLostTimes(pRole->GetID(), GetSampleSceneID(pRow)) <= 0)
	{
		return KKSG::ERR_SCENE_TODYCOUNTLIMIT;	
	}

	return KKSG::ERR_SUCCESS;
}

int TaskTeamHandler::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
	int ret = ITeamTypeHandlerBase::CheckEnterCondtion(pRole, pRow, pExtrData);
	TJUDGERET(ret)

	if(pExtrData->IsHelper())
	{
		return KKSG::ERR_SUCCESS;
	}

	UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(pRow->DNExpeditionID);
	UINT32 leftCount = pRole->Get<StageMgr>()->GetCanPlayCount(sceneID);
	if(0 == leftCount)
	{
		return KKSG::ERR_SCENE_TODYCOUNTLIMIT;	
	}

	return KKSG::ERR_SUCCESS;
}

int WeekEnd4v4TeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
    int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
    if (KKSG::ERR_SUCCESS != ret)
    {
        return ret;
    }
    // 检查进入条件
    //if(!CHolidayStage::Instance()->IsHolidaySecneId(GetSampleSceneID(pRow)))
    //{
    //    return KKSG::ERR_FAILED;// TODO
    //}
    return KKSG::ERR_SUCCESS;
}

int WeekEnd4v4TeamHandler::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
    int ret = ITeamTypeHandlerBase::CheckEnterCondtion(pRole, pRow, pExtrData);
    if (KKSG::ERR_SUCCESS != ret)
    {
        return ret;
    }

    if(pExtrData->IsHelper())
    {
        return KKSG::ERR_SUCCESS;
    }

    // 检查进入次数
    //if(CHolidayStage::Instance()->GetLostTimes(pRole->GetID(), GetSampleSceneID(pRow)) <= 0)
    //{
    //    return KKSG::ERR_SCENE_TODYCOUNTLIMIT;	
    //}

    return KKSG::ERR_SUCCESS;
}

int CustomBattlePkTwoTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
    int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
    if (KKSG::ERR_SUCCESS != ret)
    {
        return ret;
    }

    return KKSG::ERR_SUCCESS;
}

int CustomBattlePkTwoTeamHandler::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
    int ret = ITeamTypeHandlerBase::CheckEnterCondtion(pRole, pRow, pExtrData);
    if (KKSG::ERR_SUCCESS != ret)
    {
        return ret;
    }

    return KKSG::ERR_SUCCESS;
}

int BioHellTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_BIOHELL))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	if (!pRole->Get<SpActivityRecord>()->CheckBioHellOpen(pRow))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	return KKSG::ERR_SUCCESS;
}

int BioHellTeamHandler::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
	int ret = ITeamTypeHandlerBase::CheckEnterCondtion(pRole, pRow, pExtrData);
	if (KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}
	if (!pRole->Get<SpActivityRecord>()->CheckBioHellOpen(pRow))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}
	return KKSG::ERR_SUCCESS;
}

int MarriageCoupleTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_MARRIAGE))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	return KKSG::ERR_SUCCESS;
}

int MarriageCoupleTeamHandler::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
	return ITeamTypeHandlerBase::CheckEnterCondtion(pRole, pRow, pExtrData);
}

int MarriageTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_MARRIAGE))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	return KKSG::ERR_SUCCESS;
}

int MarriageTeamHandler::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
	return KKSG::ERR_SUCCESS;
}

int CompeteDragonTeamHandler::CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData)
{
	if(!pRole->Get<CCompeteDragonRecord>()->CheckRoleCondition(pRow->DNExpeditionID))
	{
		return KKSG::ERR_COMPETEDRAGON_WRONG_DNID;
	}
	return KKSG::ERR_SUCCESS;
}

int CompeteDragonTeamHandler::CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow)
{
	int ret = ITeamTypeHandlerBase::CheckOpenCondition(pRole, pRow);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	if(!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_COMPETEDRAGON))
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	return KKSG::ERR_SUCCESS;
}
