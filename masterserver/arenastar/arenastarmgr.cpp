#include "pch.h"
#include "arenastarmgr.h"
#include "role/role.h"
#include "ranklist/pkranklist.h"
#include "ranklist/herobattleranklist.h"
#include "ranklist/nestweekranklist.h"
#include "ranklist/leagueranklist.h"
#include "network/dblink.h"
#include "leagueteam/leagueteammgr.h"
#include "leagueteam/leagueteam.h"
#include "table/globalconfig.h"
#include "ranklist/ranklistmgr.h"
#include "arenastar/ptcm2c_arenastardatantf.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "util/XCommon.h"
#include "common/rpcm2g_msgiveitem.h"
#include "define/tlogenum.h"
#include "mail/mailconfig.h"
#include "mail/mailconfig.h"
#include "define/itemdef.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "util/gametime.h"

INSTANCE_SINGLETON(ArenaStarMgr)

ArenaStarMgr::ArenaStarMgr()
{
    m_handler = 0;
}

ArenaStarMgr::~ArenaStarMgr()
{

}

bool ArenaStarMgr::Init()
{
	if(!m_dbHander.Read())
	{
		return false;
	}

	if(!ReadSeasonGlobalData())
	{
		return false;
	}

	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	return true;
}

void ArenaStarMgr::Uninit()
{
	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}
	ClearAll();
}

void ArenaStarMgr::SaveAll2DB()
{
	SSInfo << " save all size = " << m_dbHander.WaitSize() << END;
	int WhileLimit = 10000;
	while(m_dbHander.WaitSize() > 0 && (WhileLimit--) >= 0)
	{
		m_dbHander.Update(GetGlobalConfig().ArenaStarSaveDBLimit);
	}
}

void ArenaStarMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
    if(DBLink::Instance()->IsConnected())
    {
        m_dbHander.Update(GetGlobalConfig().ArenaStarSaveDBLimit); 
    }
}

void ArenaStarMgr::OnArenaSeasonEnd(KKSG::ArenaStarType type, UINT64 ID /*= 0*/)
{
	ArenaStarRankP rankP = GetTypeRankP(type);
	if(NULL == rankP.pCurRank || NULL == rankP.pLastRank)
	{
		return;
	}

    std::vector<ASPair> rankVec;
	UINT32 MaxRankCount = UINT32(-1);

	if(KKSG::AST_LEAGUE == type)
	{
		LeagueRankList* pCurRank = dynamic_cast<LeagueRankList*>(rankP.pCurRank);
		LastWeekLeagueRankList* pLastRank = dynamic_cast<LastWeekLeagueRankList*>(rankP.pLastRank);
		if(pCurRank && pLastRank)
		{
			if(!pLastRank->ASUpdateLastWeekRankInfoLeague(pCurRank, ID))
			{
				SSWarn << " league update err type = " << type << END;
				return;
			}

			pLastRank->ASGetTopRank(MaxRankCount, rankVec);
		}
	}
	else
	{
		rankP.pCurRank->ASGetTopRank(MaxRankCount, rankVec);
		if(rankVec.empty())//数据为空 则忽略本赛季
		{
			SSWarn << " rankvec empty type = " << type << END;
			return;
		}
		rankP.pLastRank->ASUpdateLastWeekRankInfo(rankP.pCurRank);
	}

    UINT32 curSeason = GetTypeSeason(type);
    UINT32 nextSeason = curSeason + 1;

	for(size_t i = 0; i< rankVec.size(); ++i)	
    {
        UINT32 rank = rankVec[i].first;
        UINT64 roleID = rankVec[i].second;

        CheckAddRoleAndSeason(roleID);//(just in case)

        auto pRoleData = GetRoleData(roleID);
        if(pRoleData)
        {
            pRoleData->OnSeaonEnd(type, rank, nextSeason);
        }
    }

	UpdateTypeSeason(type, nextSeason);
	SetSeasonEndTime(type, nextSeason, UINT32(GameTime::GetTime()));

	auto itbegin = CRoleManager::Instance()->Begin();
	auto itend = CRoleManager::Instance()->End();
	for(auto it = itbegin; it != itend; ++it)
	{
		CRole* pRole = (*it);
		CheckAddRoleAndSeason(pRole->GetID());
		CheckRoleNotice(pRole);
	}

	SeasonPrint();//log	
}

ASRoleData* ArenaStarMgr::GetRoleData(UINT64 roleID)
{
	auto it = m_roleData.find(roleID);
	if(it != m_roleData.end())
	{
		return &(it->second);
	}
	return NULL;
}

void ArenaStarMgr::OnLogin(CRole* pRole)
{
    if(NULL == pRole)
    {
        return;
    }
    UINT64 roleID = pRole->GetID();

    CheckAddRoleAndSeason(roleID);

	CheckRoleNotice(pRole);
}

void ArenaStarMgr::AddNewData(UINT64 roleID)
{
    SSInfo << " roleID = " << roleID << END;
    
    ASRoleData data(roleID);
	if(Add(data))
	{
		AddChangeRole(roleID);
	}
}

/**
 * @brief add new role if NULL == roledata
 *        and add role season data
 *
 * @param roleID
 */
void ArenaStarMgr::CheckAddRoleAndSeason(UINT64 roleID)
{
    auto pRoleData = GetRoleData(roleID);
    if(NULL == pRoleData)// new role data (only this place)
    {
        AddNewData(roleID);
        pRoleData = GetRoleData(roleID);
    }

    if(NULL == pRoleData)
    {
		SSError << " role data = null role = " << roleID << END;
        return;
    }

	//fill all type season data
    for(int i = KKSG::ArenaStarType_MIN; i <= KKSG::ArenaStarType_MAX; ++i)
    {
        KKSG::ArenaStarType type = KKSG::ArenaStarType(i);
        UINT32 curMaxSeason = GetTypeSeason(type);
        pRoleData->CheckFillSeasonData(type, curMaxSeason);
    } 
}

void ArenaStarMgr::UpdateTypeSeason(KKSG::ArenaStarType type, UINT32 season)
{
    //SSInfo << " up season type = " << type << " season = " << season << END;

    if(season > m_typeSeason[type])
    {
        m_typeSeason[type] = season;
    }
}

UINT32 ArenaStarMgr::GetTypeSeason(KKSG::ArenaStarType type)
{
    return m_typeSeason[type];
}

void ArenaStarMgr::AddDataFromDB(KKSG::ArenaStarRoleData& dbData)
{
	ASRoleData roleData(dbData);

	//update season data
	for(int i = KKSG::ArenaStarType_MIN; i <= KKSG::ArenaStarType_MAX; ++i)
	{
		KKSG::ArenaStarType type = KKSG::ArenaStarType(i);
		UpdateTypeSeason(type, roleData.GetMaxSeason(type));
	}

	Add(roleData);
}

bool ArenaStarMgr::Add(const ASRoleData& data)
{
	auto ret = m_roleData.insert(std::make_pair(data.RoleID(), data));
	if(!ret.second)
	{
		SSError << " add role data failed role = " << data.RoleID() << END;
		return false;
	}
	return true;
}

void ArenaStarMgr::CheckRoleNotice(CRole* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	auto pRoleData = GetRoleData(pRole->GetID());
	if(NULL == pRoleData)
	{
		return;
	}

	PtcM2C_ArenaStarDataNtf ptc;	
	for(int i = KKSG::ArenaStarType_MIN; i <= KKSG::ArenaStarType_MAX; ++i)
	{
		KKSG::ArenaStarType type = (KKSG::ArenaStarType)(i);
		if(pRoleData->GetMaxSeason(type) <= 0)//还没有赛季数据
		{
			continue;
		}
		if(!pRoleData->GetNotice(type))
		{
			ptc.m_Data.add_newdata(type);
		}
	}

	pRole->Send(ptc);
}

void ArenaStarMgr::HandleDianZan(CRole* pRole, KKSG::ArenaStarType type, KKSG::ArenaStarReqRes& roRes)
{
	if(NULL == pRole)
	{
		return;
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);	

	auto pRoleData = GetRoleData(pRole->GetID());
	if(NULL == pRoleData)
	{
		return;
	}
	if(pRoleData->GetNotice(type))
	{
		return;
	}
	pRoleData->SetNotice(type, 1);
	GiveDianZanRewardGs(pRole, type);
}

void ArenaStarMgr::HandleTopRoleData(KKSG::ArenaStarType type, KKSG::ArenaStarReqRes& roRes)
{
	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	std::vector<ASPair> vec;

	ArenaStarRankP rankP = GetTypeRankP(type);
	if(NULL == rankP.pCurRank || NULL == rankP.pLastRank)
	{
		return;
	}

	rankP.pLastRank->ASGetTopRank(rankP.pLastRank->ASGetTopViewRank(), vec);

	for(size_t i = 0; i < vec.size(); ++i)
	{
		UINT32 rank = vec[i].first;
		UINT64 roleID = vec[i].second;
		auto pRoleData = GetRoleData(roleID);
		auto pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleID);
		if(NULL == pRoleData || NULL == pSummary)
		{
			continue;
		}
		auto& data = *roRes.add_toproledata();
		data.set_rank(rank);
		pRoleData->FillHistoryData(type, *data.mutable_historydata());
		pSummary->FillRoleOutLookBrief(*data.mutable_outlook());
	}

	FillSeasonTime(type, roRes);

	roRes.set_season_num(GetTypeSeason(type));
}

ArenaStarRankP ArenaStarMgr::GetTypeRankP(KKSG::ArenaStarType type)
{
	ArenaStarRankP rankP;
    switch(type)
    {
        case KKSG::AST_PK:
            {
				PkRealTimeRankList* pCurRankList = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(KKSG::PkRealTimeRank);
				LastWeekPkRankList* pLastWeekRankList = (LastWeekPkRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LastWeek_PkRank);
				if(NULL == pCurRankList || NULL == pLastWeekRankList)
				{
					SSError << " get ranklist failed arenastar type = " << type << END;
					break;
				}
				rankP.pCurRank = pCurRankList;
				rankP.pLastRank = pLastWeekRankList;
            }
            break;
		case KKSG::AST_HEROBATTLE:
            {
				HeroBattleRankList* pCurRankList = (HeroBattleRankList*)CRankListMgr::Instance()->GetRankList(KKSG::HeroBattleRank);
				LastWeekHeroBattleRankList* pLastWeekRankList = (LastWeekHeroBattleRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LastWeek_HeroBattleRank);
				if(NULL == pCurRankList || NULL == pLastWeekRankList)
				{
					SSError << " get ranklist failed arenastar type = " << type << END;
					break;
				}
				rankP.pCurRank = pCurRankList;
				rankP.pLastRank = pLastWeekRankList;
            }
            break;
        case KKSG::AST_WEEKNEST:
            {
				NestWeekRankList* pCurRankList = (NestWeekRankList*)CRankListMgr::Instance()->GetRankList(KKSG::NestWeekRank);
				LastWeekNestWeekRankList* pLastWeekRankList = (LastWeekNestWeekRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LastWeek_NestWeekRank);
				if(NULL == pCurRankList || NULL == pLastWeekRankList)
				{
					SSError << " get ranklist failed arenastar type = " << type << END;
					break;
				}
				rankP.pCurRank = pCurRankList;
				rankP.pLastRank = pLastWeekRankList;
            }
            break;
		case KKSG::AST_LEAGUE:
            {
				LeagueRankList* pCurRankList = (LeagueRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LeagueTeamRank);
				LastWeekLeagueRankList* pLastWeekRankList = (LastWeekLeagueRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LastWeek_LeagueTeamRank);
				if(NULL == pCurRankList || NULL == pLastWeekRankList)
				{
					SSError << " get ranklist failed arenastar type = " << type << END;
					break;
				}
				rankP.pCurRank = pCurRankList;
				rankP.pLastRank = pLastWeekRankList;
            }
            break;
        default:
			{
				SSError << " unknow type = " << type << END;
			}
            break;
    }
	return rankP;
}

void ArenaStarMgr::AddChangeRole(UINT64 roleID)
{
	SSInfo << " add change role = " << roleID << END;
	m_dbHander.AddChangeRole(roleID);
}

void ArenaStarMgr::SeasonPrint()
{
	for(auto it = m_typeSeason.begin(); it != m_typeSeason.end(); ++it)
	{
		SSInfo << " type = " << (it->first) << " season = " << (it->second) << END;
	}
}

void ArenaStarMgr::RolePrint()
{
	for(auto it = m_roleData.begin(); it != m_roleData.end(); ++it)
	{
		(it->second).InfoPrint();
	}
}

void ArenaStarMgr::GlobalDataPrint()
{
	for(int i = 0; i < m_seasonTime.typedata_size(); ++i)
	{
		const auto& typeData = m_seasonTime.typedata(i);
		SSDebug << " type = " << typeData.type() << END;
		for(int j = 0; j < typeData.seasontime_size(); ++j)
		{
			SSDebug << " season = " << typeData.seasontime(j).season() << " time = " << typeData.seasontime(j).endtime() << END;
		}
	}
}

void ArenaStarMgr::InfoPrint()
{
	SeasonPrint();
	RolePrint();
	GlobalDataPrint();
}

bool ArenaStarMgr::ReadSeasonGlobalData()
{
	std::string stmp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eArenaStarSeasonData);
	if(stmp.empty())
	{
		SSInfo << " first time " << END;
	}
	else
	{
		if(!m_seasonTime.ParseFromString(stmp))
		{
			SSError << " parse season global data failed " << END;
			return false;			
		}
	}
	return true;
}

void ArenaStarMgr::WriteSeasonGlobalData()
{
	std::string stmp;
	if(!m_seasonTime.SerializeToString(&stmp))
	{
		SSError << " SerializeToString failed " << END;
		return;
	}
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eArenaStarSeasonData, stmp);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eArenaStarSeasonData);
}

UINT32 ArenaStarMgr::GetSeasonEndTime(KKSG::ArenaStarType type, UINT32 season)
{
	for(int i = 0; i < m_seasonTime.typedata_size(); ++i)
	{
		if(m_seasonTime.typedata(i).type() == type)
		{
			const auto& tmp = m_seasonTime.typedata(i);
			for(int j = 0; j < tmp.seasontime_size(); ++j)
			{
				if(tmp.seasontime(j).season() == season)
				{
					return tmp.seasontime(j).endtime();
				}
			}
		}
	}
	return 0;
}

void ArenaStarMgr::SetSeasonEndTime(KKSG::ArenaStarType type, UINT32 season, UINT32 ctime)
{
	SSInfo << " set season endtime type = " << type << " season = " << season << " time = " << ctime << END;

	KKSG::ASTypeSeasonTime* pTypeData = NULL;
	for(int i = 0; i < m_seasonTime.typedata_size(); ++i)
	{
		if(m_seasonTime.typedata(i).type() == type)
		{
			pTypeData = m_seasonTime.mutable_typedata(i);
			break;
		}
	}
	if(NULL == pTypeData)
	{
		pTypeData = m_seasonTime.add_typedata();	
		pTypeData->set_type(type);
	}
	
	auto pTimeData = pTypeData->add_seasontime();
	pTimeData->set_season(season);
	pTimeData->set_endtime(ctime);

	if(pTypeData->seasontime_size() > 2)//只存最后两个赛季
	{
		for(int i = 0; i < pTypeData->seasontime_size() - 1; ++i)
		{
			pTypeData->mutable_seasontime()->SwapElements(i, i+1);
		}
		pTypeData->mutable_seasontime()->RemoveLast();
	}

	WriteSeasonGlobalData();
}

void ArenaStarMgr::FillSeasonTime(KKSG::ArenaStarType type, KKSG::ArenaStarReqRes& roRes)
{
	UINT32 season = GetTypeSeason(type);
	if(0 == season)
	{
		return;
	}
	UINT32 beginTime = GetSeasonEndTime(type, season - 1);
	UINT32 endTime = GetSeasonEndTime(type, season);
	roRes.set_seasonbegintime(beginTime);
	roRes.set_seasonendtime(endTime);
	SSDebug << " begintime = " << beginTime << " endtime = " << endTime << END;
}

void ArenaStarMgr::GiveDianZanRewardGs(CRole* pRole, KKSG::ArenaStarType type)
{
	if(NULL == pRole)
	{
		return;
	}
	RpcM2G_MsGiveItem *rpc = RpcM2G_MsGiveItem::CreateRpc();
	KKSG::MsGiveItemArg& roArg = rpc->m_oArg;
	roArg.set_roleid(pRole->GetID());
	roArg.set_reason(ItemFlow_ArenaStar);
	roArg.set_subreason(ItemFlow_ArenaStar_DianZan);
	const auto& rewards = GetGlobalConfig().ArenaStarReward;
	for(size_t i = 0; i < rewards.size(); ++i)
	{
		const Sequence<int,3>& seq = rewards[i];
		if(seq[0] == type)
		{
			KKSG::ItemBrief& item = *roArg.add_items();
			item.set_itemid(seq[1]);
			item.set_itemcount(seq[2]);
			item.set_isbind(true);
			break;
		}
	}
	pRole->SendMsgToGS(*rpc);
}

void ArenaStarMgr::GiveDianZanRewardMail(const KKSG::MsGiveItemArg& roArg)
{
	std::string mailtitle = "null";
	std::string mailcontent;
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_ArenaStar_DianZan);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
	}
	std::vector<ItemDesc> rewards;
	for(int i = 0; i < roArg.items_size(); ++i)
	{
		ItemDesc desc;
		desc.ToThis(roArg.items(i));
		rewards.push_back(desc);
	}

	MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
	maildata.SetReason(roArg.subreason());
	CMailMgr::Instance()->SendMail(roArg.roleid(), maildata);
}

void ArenaStarMgr::ClearAll()
{
	m_roleData.clear();
	m_typeSeason.clear();
	m_seasonTime.Clear();
}

//数据归0 用来测试
void ArenaStarMgr::Data2Zero()
{
	m_typeSeason.clear();

	m_seasonTime.Clear();
	WriteSeasonGlobalData();

	for(auto it = m_roleData.begin(); it != m_roleData.end(); ++it)
	{
		(it->second).Clear();
	}

	for(int type = KKSG::LastWeek_PkRank; type <= KKSG::LastWeek_LeagueTeamRank; ++type)
	{
		auto pRankList = CRankListMgr::Instance()->GetRankList(type);
		pRankList->Clear();
		pRankList->GetDbHandler()->SendToDB();
	}
	for(auto it = CRoleManager::Instance()->Begin(); it != CRoleManager::Instance()->End(); ++it)
	{
		CheckRoleNotice(*it);
	}
}

