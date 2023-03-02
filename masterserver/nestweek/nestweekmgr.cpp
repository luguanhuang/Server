#include "pch.h"
#include "nestweekmgr.h"
#include <ctime>
#include "table/expeditionconfigmgr.h"
#include "util/XCommon.h"
#include "levelseal/levelsealMgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/nestweekranklist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/systemiddef.h"
#include "event/eventmgr.h"
#include "arenastar/arenastarmgr.h"
#include "network/dblink.h"
#include "util/gametime.h"
#include "other/servercrossdatamgr.h"

INSTANCE_SINGLETON(NestWeekMgr)

NestWeekMgr::NestWeekMgr()
{
	m_handler = 0;
	m_lastWeekUpTime = 0;
	m_lastSceneIndex = -1;
	m_lastUseSealType = 0;
}

NestWeekMgr::~NestWeekMgr()
{

}

bool NestWeekMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);

	ReadDbData();

	return true;
}

void NestWeekMgr::Uninit()
{
	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}

}

void NestWeekMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if(!DBLink::Instance()->IsConnected())
	{
		return;
	}
	time_t now = GameTime::GetTime();

	if(0 == m_lastWeekUpTime)//第一次启服
	{
		m_lastWeekUpTime = UINT32(now);	
		WriteDbData();
		return ;
	}

	UINT32 oldExp = GetCurExp();

	if(!XCommon::IsWeekSame(m_lastWeekUpTime, now))
	{
		m_lastWeekUpTime = UINT32(now);

		WeekZero();

		WriteDbData();
	}

	CheckOpen();

	UINT32 newExp = GetCurExp();
	if(oldExp != newExp)
	{
		ServerCrossDataSyn::SynDataToWorld(0, SCDST_ONLY_TS);
	}
}

void NestWeekMgr::WeekZero()
{
	SSInfo << __FUNCTION__ << END;
	if(m_lastSceneIndex < 0)
	{
		return ;//未开启
	}

	ArenaStarMgr::Instance()->OnArenaSeasonEnd(KKSG::AST_WEEKNEST);

	RankClear();	

	//更新关卡
	std::vector<int> expIDs;
	int sealType = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	ExpeditionConfigMgr::Instance()->GetNestWeekExp(sealType, expIDs);
	if(expIDs.empty())
	{
		SSError << __FUNCTION__ << " sealtype = " << sealType << END;
		return ;
	}
	if(sealType == m_lastUseSealType)
	{
		m_lastSceneIndex = (m_lastSceneIndex + 1) % (expIDs.size());
	}
	else
	{
		m_lastUseSealType = sealType;
		m_lastSceneIndex = 0;
	}

	m_hintRecord.clear();
	OnlineHint();
}

void NestWeekMgr::ReadDbData()
{
	string tmp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eWeekNestLastUpTime);
	if(!tmp.empty())
	{
		m_lastWeekUpTime = convert<UINT32>(tmp);
	}
	else
	{
		SSWarn << __FUNCTION__ << END;
	}

	tmp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eWeekNestLastSceneIndex);
	if(!tmp.empty())
	{
		m_lastSceneIndex = convert<int>(tmp);
	}
	else
	{
		SSWarn << __FUNCTION__ << END;
	}

	tmp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eWeekNestLastSealType);
	if(!tmp.empty())
	{
		m_lastUseSealType = convert<int>(tmp);
	}
	else
	{
		SSWarn << __FUNCTION__ << END;
	}

	SSInfo << " " << m_lastWeekUpTime << " " << m_lastSceneIndex << " " << m_lastUseSealType << END;
}

void NestWeekMgr::WriteDbData()
{
	SSInfo << " write " << m_lastWeekUpTime << " " << m_lastSceneIndex << " " << m_lastUseSealType << END;

	CGlobalConfigDBBuild::Instance()->SetConfigValue(eWeekNestLastUpTime, ToString(m_lastWeekUpTime));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eWeekNestLastUpTime);
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eWeekNestLastSceneIndex, ToString(m_lastSceneIndex));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eWeekNestLastSceneIndex);
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eWeekNestLastSealType, ToString(m_lastUseSealType));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eWeekNestLastSealType);
}

void NestWeekMgr::CheckOpen()
{
	if(m_lastSceneIndex >= 0)//已经开启
	{
		if(0 == m_lastUseSealType)//已开服问题修正
		{
			SSInfo << " before " << m_lastWeekUpTime << " " << m_lastSceneIndex << " " << m_lastUseSealType << END;
			m_lastUseSealType = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;		
			SSInfo << " after " << m_lastWeekUpTime << " " << m_lastSceneIndex << " " << m_lastUseSealType << END;
			WriteDbData();
		}

		return;
	}
	//检测开启
	std::vector<int> expIDs;
	UINT32 curSealType = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	ExpeditionConfigMgr::Instance()->GetNestWeekExp(curSealType, expIDs);
	if(expIDs.empty())
	{
		return ;
	}
	m_lastSceneIndex = 0;//开启	
	m_lastUseSealType = curSealType;
	
	WriteDbData();

	OnlineHint();
}

void NestWeekMgr::RankClear()
{
	auto pRanklist = (NestWeekRankList*)CRankListMgr::Instance()->GetRankList(KKSG::NestWeekRank);
	if(NULL == pRanklist)
	{
		return ;
	}
	
	//NWRKey roles;
	//pRanklist->GetRoleByRank(1, roles);
	//GiveFirstRankReward(roles);

	std::vector<NestWeekRankInfo*> vec;
	pRanklist->GetAllInfo(vec);
	std::map<UINT64,UINT32> mapDesig;
	for(size_t i = 0; i < vec.size(); ++i)
	{
		if(vec[i])
		{
			for(auto iter = vec[i]->GetKey().begin(); iter!=vec[i]->GetKey().end(); iter++)
			{
				auto itFind = mapDesig.find(*iter);
				if (itFind==mapDesig.end())
				{
					LogInfo("NestWeekMgrDesig-----nRoleID=%llu nRank=%d",*iter,i+1);
					mapDesig.insert(std::pair<UINT64,UINT32>(*iter,i+1));
					EventMgr::Instance()->AddEvent(*iter, DESIGNATION_COM_NESTWEEK,i+1);
				}
			}
		}
	}

	pRanklist->Clear();
	
}

void NestWeekMgr::GiveRankReward(const std::vector<UINT64>& roles, UINT32 rank)
{

}

//void NestWeekMgr::GiveFirstRankReward(std::vector<UINT64>& roles)
//{
//	//每周结算 第一名 roles
//	for(size_t i = 0; i < roles.size(); ++i)
//	{
//		if (i == 0)
//		{
//			EventMgr::Instance()->AddEvent(roles[i], DESIGNATION_COM_NESTWEEK,1);
//		}else if(i < 10)
//		{
//			EventMgr::Instance()->AddEvent(roles[i], DESIGNATION_COM_NESTWEEK,2);
//		}else if(i < 100)
//		{
//			EventMgr::Instance()->AddEvent(roles[i], DESIGNATION_COM_NESTWEEK,3);
//		}else
//		{
//			EventMgr::Instance()->AddEvent(roles[i], DESIGNATION_COM_NESTWEEK,4);
//		}
//		SSInfo << " role = " << roles[i] << END;
//	}
//}

void NestWeekMgr::CheckHint(CRole* pRole)
{
	if(m_lastSceneIndex < 0)//未开启
	{
		return;
	}
	if(NULL == pRole)
	{
		return ;
	}
	if(m_hintRecord.find(pRole->GetID()) != m_hintRecord.end())
	{
		return ;
	}

	m_hintRecord.insert(pRole->GetID());

	//SSDebug << __FUNCTION__ << " role = " << pRole->GetID() << END;
	pRole->HintNotify(SYS_WEEK_NEST_ID, false); 
}

void NestWeekMgr::TestSetNextWeek()
{
	if(m_lastWeekUpTime)
	{
		m_lastWeekUpTime -= (XCommon::GetOneDayTime() * 7);
	}
}

int NestWeekMgr::GetCurExp()
{
	if(m_lastSceneIndex < 0)
	{
		return 0;
	}
	
	std::vector<int> expIDs;
	ExpeditionConfigMgr::Instance()->GetNestWeekExp(m_lastUseSealType, expIDs);
	if(expIDs.empty())
	{
		return 0;
	}

	if(m_lastSceneIndex >= (int)expIDs.size())
	{
		return expIDs[0];
	}

	return expIDs[m_lastSceneIndex];
}

void NestWeekMgr::OnlineHint()
{
	auto itbegin = CRoleManager::Instance()->Begin();
	auto itend = CRoleManager::Instance()->End();
	for(auto it = itbegin; it != itend; ++it)
	{
		CheckHint(*it);
	}
}
