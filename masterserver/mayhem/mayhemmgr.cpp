#include "pch.h"
#include "mayhemmgr.h"
#include "role/rolemanager.h"
#include "table/globalconfig.h"
#include "bigmelee/ptcm2k_bmenterreadyscenentf.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "role/rolemanager.h"
#include "network/worldlink.h"
#include "../config.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "table/MultActivityMgr.h"
#include "table/OpenSystemMgr.h"
#include "util/timespecificmgr.h"
#include "notice/notice.h"
#include "define/systemiddef.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "role/role.h"
#include "ranklist/briefranklist.h"
#include "ranklist/ranklistmgr.h"
#include "scene/sceneswitch.h"

INSTANCE_SINGLETON(MayhemMsMgr)

MayhemMsMgr::MayhemMsMgr()
{
	time_handler_ = INVALID_HTIMER;
	m_nMaxPlayers = 0;
}

MayhemMsMgr::~MayhemMsMgr()
{

}

bool MayhemMsMgr::Init()
{
	if (time_handler_ == INVALID_HTIMER)
	{
		time_handler_ = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	}
	event_flag_ = false;
	timer_id_.clear();

	m_Data.Clear();

	m_hStartIcon	=  INVALID_TIMESPECIFIC;
	m_nStep			=  0;
	m_nDisIconTime	=  0;
	m_nJoinPlayers  =  0;
	bool bRet = LoadFile();
	return bRet;
}


void MayhemMsMgr::CloseTimerHandle()
{
	if (m_hStartIcon!=INVALID_TIMESPECIFIC)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(m_hStartIcon);
		m_hStartIcon = INVALID_TIMESPECIFIC;
	}
}


void MayhemMsMgr::Uninit()
{
	if (time_handler_ != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(time_handler_);
		time_handler_ = INVALID_HTIMER;	
	}
}


void MayhemMsMgr::PartCutScene()
{
	UINT32 nTotal = 0;
	auto iter = m_listBigMelee.begin();
	while (iter!=m_listBigMelee.end())
	{
		EnterScene(*iter);
		nTotal += iter->roles_size();
		m_listBigMelee.erase(iter++);
		LogDebug("cutscene players=%d time=%d",nTotal,GameTime::GetTime());
		if (nTotal>=GetGlobalConfig().BigMeleeMSCutSceneRoles)
			return;
	}
}

bool MayhemMsMgr::LoadFile()
{
	std::string szMaxPlayerNum = MSConfig::Instance()->ReadFileContent("serveronly/bigmelee.txt");
	LogInfo("ReadConfig=%d",atoi(szMaxPlayerNum.c_str()));
	m_nMaxPlayers = atoi(szMaxPlayerNum.c_str());
	if (m_nMaxPlayers==0)
	{
		return false;
	}
	return true;
}


void MayhemMsMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	PartCutScene();
	UINT32 week_day = XCommon::GetWeekNum(GameTime::GetTime());
	if (!MultActivityMgr::Instance()->IsActivityOnDay(MULT_ACTIVITY_BIGMELEE, week_day))
	{
		return ;
	}

	// 检查活动是否开启
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return;
	}

	time_t now_time =  GameTime::GetTime();
	if (m_nDisIconTime&&now_time >= m_nDisIconTime)
	{
		m_nDisIconTime = 0;
		SendIcon(SYS_BIGMELEEEND,KKSG::HICONS_END);
		m_setRoleID.clear();
	}

	UINT32 open_time = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_BIGMELEE);
	if ((UINT32)now_time >= open_time)
	{
		return ;
	}
	
	// 凌晨5点才算跨天，所以5点前，会认为活动还未开始，只能检查活动状态了。
	KKSG::MulActivityTimeState activity_state = MultActivityMgr::Instance()->GetOpenTimeState(MULT_ACTIVITY_BIGMELEE);
	if (KKSG::MULACTIVITY_BEfOREOPEN != activity_state)
	{
		return;
	}

	if (event_flag_)
	{
		return;
	}	

	SetTimeEvent();
}

void MayhemMsMgr::OnLogin(CRole *pRole)
{
	// 检查活动是否开启
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
		return;
	if(pRole->IsSystemOpened(SYS_BIGMELEESTART) == false)
		return;
	if(m_nStep == 1)
	{
		PtcM2C_HallIconMNtf ptc;
		ptc.m_Data.set_systemid(SYS_BIGMELEESTART);
		ptc.m_Data.set_state(KKSG::HICONS_BEGIN);
		pRole->Send(ptc);
	}else if (m_nStep == 3)
	{
		auto iter = m_setRoleID.find(pRole->GetID());
		if (iter!=m_setRoleID.end())
		{
			PtcM2C_HallIconMNtf ptc;
			ptc.m_Data.set_systemid(SYS_BIGMELEEEND);
			ptc.m_Data.set_state(KKSG::HICONS_BEGIN);
			pRole->Send(ptc);
		}
	}
}


void MayhemMsMgr::DelRole(UINT64 nRole)
{
	auto iter = m_setRoleID.find(nRole);
	if (iter!=m_setRoleID.end())
	{
		m_setRoleID.erase(nRole);
	}
}




void MayhemMsMgr::SetTimeEvent()
{
	time_t now_time =  GameTime::GetTime();
	UINT32 open_time = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_BIGMELEE);
	if ((UINT32)now_time >= open_time)
	{
		return ;
	}
	// 先清理
	ClearNoticeTimeInfo();

	UINT32 end_time = MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_BIGMELEE);

	tm* local_time = localtime(&now_time);	
	// 小时
	auto it = GetGlobalConfig().BigMeleePreHourNoticeList.begin();
	auto it_end = GetGlobalConfig().BigMeleePreHourNoticeList.end();
	for (; it != it_end; ++it)
	{
		if (((UINT32)(local_time->tm_hour) < (*it)) && ((*it) < 24))
		{
			TimeSpecific notice_time(*it, 0, 0);
			UINT32 timer_id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_time, std::bind(&MayhemMsMgr::PreNoticeByHour, this));
			timer_id_.insert(timer_id);
		}
	}

	if (open_time > now_time )
	{
		if (open_time - now_time > 30*60)
		{
			time_t pre30minute_time =  open_time - 30*60;
			tm* tm_30time = localtime(&pre30minute_time);	
			TimeSpecific notice_minute(tm_30time->tm_hour, tm_30time->tm_min, 0);
			UINT32 timer_30id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy30Minute, this));
			timer_id_.insert(timer_30id);
		}
		if (open_time - now_time > 20*60)
		{
			time_t pre20minute_time =  open_time - 20*60;
			tm* tm_20time = localtime(&pre20minute_time);	
			TimeSpecific notice_minute(tm_20time->tm_hour, tm_20time->tm_min, 0);
			UINT32 timer_20id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy20Minute, this));
			timer_id_.insert(timer_20id);
		}
		if (open_time - now_time > 10*60)
		{
			time_t pre10minute_time =  open_time - 10*60;
			tm* tm_10time = localtime(&pre10minute_time);	
			TimeSpecific notice_minute(tm_10time->tm_hour, tm_10time->tm_min, 0);
			UINT32 timer_10id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy10Minute, this));
			timer_id_.insert(timer_10id);
		}
		if (open_time - now_time > 3*60)
		{
			time_t pre3minute_time =  open_time - 3*60;
			tm* tm_3time = localtime(&pre3minute_time);	
			TimeSpecific notice_minute(tm_3time->tm_hour, tm_3time->tm_min, 0);
			UINT32 timer_3id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy3Minute, this));
			timer_id_.insert(timer_3id);
		}
		if (open_time - now_time > 2*60)
		{
			time_t pre2minute_time =  open_time - 2*60;
			tm* tm_2time = localtime(&pre2minute_time);	
			TimeSpecific notice_minute(tm_2time->tm_hour, tm_2time->tm_min, 0);
			UINT32 timer_2id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy2Minute, this));
			timer_id_.insert(timer_2id);
		}
		if (open_time - now_time > 1*60)
		{
			time_t pre1minute_time =  open_time - 1*60;
			tm* tm_1time = localtime(&pre1minute_time);	
			TimeSpecific notice_minute(tm_1time->tm_hour, tm_1time->tm_min, 0);
			UINT32 timer_1id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy1Minute, this));
			timer_id_.insert(timer_1id);
		}

		// 开始倒计时
		if (end_time - now_time > 3*60)
		{
			time_t pre3start_time =  end_time - 3*60;
			tm* tm_start3time = localtime(&pre3start_time);	
			TimeSpecific notice_minute(tm_start3time->tm_hour, tm_start3time->tm_min, 0);
			UINT32 timer_start3id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy3Start, this));
			timer_id_.insert(timer_start3id);
		}
		if (end_time - now_time > 2*60)
		{
			time_t pre2start_time =  end_time - 2*60;
			tm* tm_start2time = localtime(&pre2start_time);	
			TimeSpecific notice_minute(tm_start2time->tm_hour, tm_start2time->tm_min, 0);
			UINT32 timer_start2id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy2Start, this));
			timer_id_.insert(timer_start2id);
		}
		if (end_time - now_time > 1*60)
		{
			time_t pre1start_time =  end_time - 1*60;
			tm* tm_start1time = localtime(&pre1start_time);	
			TimeSpecific notice_minute(tm_start1time->tm_hour, tm_start1time->tm_min, 0);
			UINT32 timer_start1id = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::PreNoticeBy1Start, this));
			timer_id_.insert(timer_start1id);
		}

		if (end_time - now_time > 0)
		{
			time_t start_notice_time =  end_time ;
			tm* tm_starttime = localtime(&start_notice_time);	
			TimeSpecific notice_minute(tm_starttime->tm_hour, tm_starttime->tm_min, 0);
			UINT32 timer_startid = CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&MayhemMsMgr::NoticeStart, this));
			timer_id_.insert(timer_startid);
		}
	}
	
	event_flag_ = true;
}

bool MayhemMsMgr::PreNoticeByHour()
{	
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	time_t begin_time = (time_t)MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_BIGMELEE);
	tm* start_time = localtime(&begin_time);

	Notice notice(MayHemHourNotice);
	notice.Replace("{0}",ToString(start_time->tm_hour));
	notice.Replace("{1}",ToString(start_time->tm_min));
	notice.Send();

	return true;
}
bool MayhemMsMgr::PreNoticeBy30Minute()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}

	Notice notice(MayHemMinuteNotice);		
	notice.Replace("{0}", "30");
	notice.Send();
	return true;
}
bool MayhemMsMgr::PreNoticeBy20Minute()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}

	Notice notice(MayHemMinuteNotice);		
	notice.Replace("{0}", "20");
	notice.Send();
	return true;
}
bool MayhemMsMgr::PreNoticeBy10Minute()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	Notice notice(MayHemMinuteNotice);		
	notice.Replace("{0}", "10");
	notice.Send();
	return true;
}
bool MayhemMsMgr::PreNoticeBy3Minute()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	Notice notice(MayHemMinuteNotice);		
	notice.Replace("{0}", "3");
	notice.Send();
	return true;
}
bool MayhemMsMgr::PreNoticeBy2Minute()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	Notice notice(MayHemMinuteNotice);		
	notice.Replace("{0}", "2");
	notice.Send();
	return true;
}
bool MayhemMsMgr::PreNoticeBy1Minute()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	Notice notice(MayHemMinuteNotice);		
	notice.Replace("{0}", "1");
	notice.Send();
	return true;
}
bool MayhemMsMgr::PreNoticeBy3Start()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	Notice notice(MayHemPreStartNotice);		
	notice.Replace("{0}", "3");
	notice.Send();
	return true;
}
bool MayhemMsMgr::PreNoticeBy2Start()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	Notice notice(MayHemPreStartNotice);		
	notice.Replace("{0}", "2");
	notice.Send();
	return true;
}
bool MayhemMsMgr::PreNoticeBy1Start()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	Notice notice(MayHemPreStartNotice);		
	notice.Replace("{0}", "1");
	notice.Send();
	return true;
}

bool MayhemMsMgr::NoticeStart()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
	{
		return true;
	}
	Notice notice(MayHemStartNotice);		
	notice.Send();	

	// 正式开始后，注册的通知都已经发了，可以清理了
	ClearNoticeTimeInfo();

	return true;
}

void MayhemMsMgr::ClearNoticeTimeInfo()
{
	for (auto it = timer_id_.begin(); it != timer_id_.end(); ++it)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*it);
	}
	event_flag_ = false;
	return;
}

void MayhemMsMgr::SetReadyScene(const KKSG::BMCrossNtfData &data)
{
	m_Data.CopyFrom(data);
	LogInfo("create readyscene line=%d scene=%d mapid=%u",data.gsline(),data.sceneid(),data.mapid());
}

const KKSG::BMCrossNtfData& MayhemMsMgr::GetBMCrossNtfData()
{
	return m_Data;
}

void MayhemMsMgr::SendIcon(UINT32 nSysID,KKSG::HallIconState state)
{
	for(auto iter = CRoleManager::Instance()->Begin(); iter!=CRoleManager::Instance()->End();iter++)
	{
		CRole* pRole = *iter;
		if (pRole->IsSystemOpened(nSysID))
		{
			if (nSysID == SYS_BIGMELEEEND&& state == KKSG::HICONS_BEGIN)
			{
				auto siter = m_setRoleID.find(pRole->GetID());
				if (siter!=m_setRoleID.end())
				{
					PtcM2C_HallIconMNtf ptc;
					ptc.m_Data.set_systemid(nSysID);
					ptc.m_Data.set_state(state);
					pRole->Send(ptc);
				}
			}else
			{
				PtcM2C_HallIconMNtf ptc;
				ptc.m_Data.set_systemid(nSysID);
				ptc.m_Data.set_state(state);
				pRole->Send(ptc);
			}

		}
	}
}

void MayhemMsMgr::OnStart()
{
	m_listBigMelee.clear();
	// 检查活动是否开启  检查是否达到开放等级
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART) || !CanOpenActivity())
	{
		return;
	}

	m_setRoleID.clear();
	// 跑马灯
	Notice notice(MayHemApproachtNotice);		
	notice.Send();

	CloseTimerHandle();

	SendIcon(SYS_BIGMELEESTART,KKSG::HICONS_BEGIN);

	TimeSpecific startts(MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_BIGMELEE));
	m_hStartIcon = CTimeSpecificMgr::Instance()->RegisterEvent(startts, std::bind(&MayhemMsMgr::ShowStartIcon, this, 0));
	m_nStep			= 1;
	m_nDisIconTime	= 0;
	m_setRoleID.clear();
	m_nJoinPlayers = 0;

}

bool MayhemMsMgr::ShowStartIcon(UINT32 nOpen)
{
	m_Data.Clear();
	m_nStep = 2;
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEESTART))
		return true;
	SendIcon(SYS_BIGMELEESTART,KKSG::HICONS_END);
	return true;
}

bool MayhemMsMgr::ShowAwardIcon()
{
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BIGMELEEEND))
		return true;
	SendIcon(SYS_BIGMELEEEND,KKSG::HICONS_BEGIN);
	m_nStep = 3;
	m_nDisIconTime = GameTime::GetDayBeginTime(GameTime::GetTime()) + 24*3600;
	return true;
}


bool MayhemMsMgr::IsFightMap(UINT32 nMapID)
{
 	auto iter =  std::find(GetGlobalConfig().BigMeleeFightScene.begin(), GetGlobalConfig().BigMeleeFightScene.end(), nMapID);
	if (iter == GetGlobalConfig().BigMeleeFightScene.end())
	{
		return false;
	}
	return true;
}

void MayhemMsMgr::TestTask()
{
	for (int i= 0; i < 999; i++)
	{
		PtcM2K_BMEnterReadySceneNtf ntf;
		ntf.m_Data.set_optype(1);
		KKSG::BMRoleEnter *pBMEnter =  ntf.m_Data.mutable_roles();
		std::stringstream ss;
		ss<<i;
		ss<<"-";
		ss<<"test";
		pBMEnter->set_name(ss.str());
		pBMEnter->set_roleid(MakeUINT64(TimeUtil::GetTime(),i));
		pBMEnter->set_level(50);
		ntf.m_Data.set_svrid(MSConfig::Instance()->GetServerID());
		WorldLink::Instance()->SendTo(ntf);
	}
}


void MayhemMsMgr::OnMapChange(CRole *pRole,UINT32 oldMapID, UINT32 newMapID)
{
	//离开等待场景
	if (oldMapID == GetGlobalConfig().BigMeleeReadyScene && !IsFightMap(newMapID))
	{
		PtcM2K_BMEnterReadySceneNtf ntf;
		ntf.m_Data.set_optype(2);
		KKSG::BMRoleEnter *pBMEnter =  ntf.m_Data.mutable_roles();
		pBMEnter->set_name(pRole->GetName());
		pBMEnter->set_roleid(pRole->GetID());
		CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
		if (pRoleSum)
			pBMEnter->set_level(pRoleSum->GetLevel());
		ntf.m_Data.set_svrid(MSConfig::Instance()->GetServerID());
		WorldLink::Instance()->SendTo(ntf);
		if (m_nJoinPlayers>0)
			m_nJoinPlayers--;
		LogInfo("MayhemMsMgr::OnMapChange playernum=%d",m_nJoinPlayers);
	}
	//进入等待场景
	if (oldMapID != GetGlobalConfig().BigMeleeReadyScene && newMapID==GetGlobalConfig().BigMeleeReadyScene)
	{
		PtcM2K_BMEnterReadySceneNtf ntf;
		ntf.m_Data.set_optype(1);
		KKSG::BMRoleEnter *pBMEnter =  ntf.m_Data.mutable_roles();
		pBMEnter->set_name(pRole->GetName());
		pBMEnter->set_roleid(pRole->GetID());
		CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
		if (pRoleSum)
			pBMEnter->set_level(pRoleSum->GetLevel());
		ntf.m_Data.set_svrid(MSConfig::Instance()->GetServerID());
		WorldLink::Instance()->SendTo(ntf);
		m_nJoinPlayers++;
		LogInfo("MayhemMsMgr::OnMapChange playernum=%d",m_nJoinPlayers);
	}

	if (IsFightMap(oldMapID)&& !IsFightMap(newMapID))
	{
		PtcM2K_BMEnterReadySceneNtf ntf;
		ntf.m_Data.set_optype(3);
		KKSG::BMRoleEnter *pBMEnter =  ntf.m_Data.mutable_roles();
		pBMEnter->set_name(pRole->GetName());
		pBMEnter->set_roleid(pRole->GetID());
		CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
		if (pRoleSum)
			pBMEnter->set_level(pRoleSum->GetLevel());
		ntf.m_Data.set_svrid(MSConfig::Instance()->GetServerID());
		WorldLink::Instance()->SendTo(ntf);
	}
}

void MayhemMsMgr::AddRoleID(UINT64 nRoleID)
{
	m_setRoleID.insert(nRoleID);
}

void MayhemMsMgr::AddCutScene(const KKSG::EnterBMFightScene& data)
{
	m_listBigMelee.push_back(data);
}

void MayhemMsMgr::EnterScene(const KKSG::EnterBMFightScene& data)
{
	for (int i = 0; i < data.roles_size(); i++)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(data.roles(i).roleid());
		if (NULL != pRole)
		{
			KKSG::SceneSwitchData info;
			CSceneSwitch::EnterScene(pRole, data.sceneid(), info, data.gsline(), true, data.mapid());
			LogInfo("enter fight scene role=%llu sceneid=%d gsline=%d time=%llu",data.roles(i).roleid(),data.sceneid(),data.gsline(),TimeUtil::GetMilliSecond());
		}else
		{
			LogError("enter fight scene role null=%llu",data.roles(i).roleid());
		}
	}
}



bool MayhemMsMgr::IsCanJoin()
{
	if (m_nJoinPlayers>=m_nMaxPlayers)
	{
		LogWarn("current join players=%u maxplayers=%u",m_nJoinPlayers,m_nMaxPlayers);
		return false;
	}

	return true;
}

bool MayhemMsMgr::CanOpenActivity()
{
	CBriefRankList* ranklist = (CBriefRankList*)CRankListMgr::Instance()->GetRankList(KKSG::LevelRank);
	if (NULL == ranklist)
	{
		SSError << " find not CBriefRankInfo " << END;
		return false ;
	}

	const CBriefRankInfo* rank_info = ranklist->GetInfoByRank(1);
	if (NULL == rank_info)
	{
		return false;
	}
	if (rank_info->m_level >= GetGlobalConfig().BigMeleeNeedPlayLevel)
	{
		return true;
	}

	return false;
}