#include "pch.h"
#include "battlefieldmgr.h"
#include "table/globalconfig.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "battlefield/ptcm2k_battlefieldeventreq.h"
#include "battlefield/ptcm2g_battlefiedlreadyscenegsntf.h"
#include "battlefield/ptcm2g_battlefailinfogsntf.h"
#include "../config.h"
#include "network/worldlink.h"
#include "table/globalconfig.h"
#include "scene/scenecreate.h"
#include "scene/sceneswitch.h"
#include "util/timespecificmgr.h"
#include "notice/notice.h"
#include "util/gametime.h"
#include "table/MultActivityMgr.h"
#include "util/XCommon.h"
#include "reswar/ptcm2c_halliconmntf.h"
#include "scene/scene.h"
#include "scene/gsmanager.h"
#include "network/gslink.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"


INSTANCE_SINGLETON(BattleFieldMgr)

BattleFieldMgr::BattleFieldMgr()
{
	m_LastUpdateTime = 0;
	time_handler_ = INVALID_HTIMER;
	m_nReadySceneID	= 0;
	m_nReadyLine    = 0;
	m_nRound        = 0;
}

BattleFieldMgr::~BattleFieldMgr()
{

}

bool BattleFieldMgr::Init()
{
	if (time_handler_ == INVALID_HTIMER)
	{
		time_handler_ = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	}
	bool bRet = LoadFile();
	return bRet;
}


void BattleFieldMgr::Uninit()
{
	if (time_handler_ != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(time_handler_);
		time_handler_ = INVALID_HTIMER;	
	}
}


bool BattleFieldMgr::LoadFile()
{
	return true;
}


void BattleFieldMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	//定时上报
	time_t nowtime = GameTime::GetTime();
	if (!XCommon::IsDaySame(m_LastUpdateTime, nowtime))
	{
		ClearTimer();
		m_LastUpdateTime = nowtime;
		KKSG::MulActivityTimeState activity_state = MultActivityMgr::Instance()->GetOpenTimeState(MULT_ACTIVITY_BATTLEFIELD);
		if (KKSG::MULACTIVITY_BEfOREOPEN != activity_state)
		{
			return;
		}
		LoadTimer();
	}
}

void BattleFieldMgr::OnLogin(CRole *pRole)
{
	if (!MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_BATTLEFIELD))
	{
		return;
	}
	if (!IsLevelOk(pRole))
	{
		return;
	}
	SendIcon(pRole, SYS_BATTLEFIELD, KKSG::HICONS_BEGIN);
}

bool BattleFieldMgr::IsLevelOk(CRole *pRole)
{
	CRoleSummary* pSum	= CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	if (pSum)
	{
		if (pSum->GetLevel()>=GetGlobalConfig().BattleFieldLevel[0][0])
		{
			return true;
		}
	}
	return false;
}


void BattleFieldMgr::OnMapChange(CRole *pRole,UINT32 oldMapID, UINT32 newMapID)
{
	if (oldMapID != GetGlobalConfig().BattleFieldReadyScene && newMapID == GetGlobalConfig().BattleFieldReadyScene)
	{
		ReportRoleInfo(pRole,KKSG::BATTLE_FIELD_READY_ENTER);
	}else if (oldMapID == GetGlobalConfig().BattleFieldReadyScene && !IsFightMap(newMapID))
	{
		ReportRoleInfo(pRole,KKSG::BATTLE_FIELD_READY_LEAVE);
	}
}

bool BattleFieldMgr::IsFightMap(UINT32 nMapID)
{
	auto iter =  std::find(GetGlobalConfig().BattleFieldFightScene.begin(), GetGlobalConfig().BattleFieldFightScene.end(), nMapID);
	if (iter == GetGlobalConfig().BattleFieldFightScene.end())
	{
		return false;
	}
	return true;
}


void BattleFieldMgr::TestTask(UINT32 nPlayer)
{
	for (int i= 0; i < nPlayer; i++)
	{
		PtcM2K_BattleFieldEventReq req;
		req.m_Data.set_event(KKSG::BATTLE_FIELD_READY_ENTER);
		KKSG::BattleFieldRoleInfo *pRoleInfo = req.m_Data.mutable_roleinfo();
		pRoleInfo->set_roleid(MakeUINT64(TimeUtil::GetTime(),i));
		std::stringstream ss;
		ss<<i;
		ss<<"-";
		ss<<"test";
		pRoleInfo->set_name(ss.str());
		pRoleInfo->set_svrid(MSConfig::Instance()->GetServerID());
		pRoleInfo->set_job(1);
		pRoleInfo->set_level(55);
		WorldLink::Instance()->SendTo(req);
	}
}


void BattleFieldMgr::ReportRoleInfo(CRole *pRole,KKSG::BattleFieldType type)
{
	PtcM2K_BattleFieldEventReq req;
	req.m_Data.set_event(type);
	KKSG::BattleFieldRoleInfo *pRoleInfo = req.m_Data.mutable_roleinfo();
	pRoleInfo->set_roleid(pRole->GetID());
	if (type == KKSG::BATTLE_FIELD_READY_ENTER)
	{
		pRoleInfo->set_name(pRole->GetName());
		pRoleInfo->set_svrid(MSConfig::Instance()->GetServerID());
		CRoleSummary* pRoleSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
		if (pRoleSum)
		{
			pRoleInfo->set_job(pRoleSum->GetProfession());
			pRoleInfo->set_level(pRoleSum->GetLevel());
		}
	}
	WorldLink::Instance()->SendTo(req);
}

void BattleFieldMgr::OnStart()
{
	// 检查活动是否开启
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BATTLEFIELD))
	{
		return;
	}


	CreateSceneReadyBegin();
	Notice notice(BattleFieldStartNotice);
	notice.Send();
	SendIconToAll(SYS_BATTLEFIELD, KKSG::HICONS_BEGIN);
}


void BattleFieldMgr::OnStop()
{
	// 检查活动是否开启
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BATTLEFIELD))
	{
		return;
	}

	m_nReadySceneID = 0;
	SendIconToAll(SYS_BATTLEFIELD, KKSG::HICONS_END);
}


bool BattleFieldMgr::CreateSceneReadyBegin()
{
	KKSG::CreateBattleParam param;
	KKSG::BFRoleEnter *pInfo =  param.mutable_bffight();
	bool flag = CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().BattleFieldReadyScene, &ICreateBattleFieldReadyListener::GlobalCreateBattleFieldReadyListener, param);	
	return flag;
}

void BattleFieldMgr::CreateSceneWaitEnd(UINT32 nSceneType, UINT32 nLineID, UINT32 nSceneID)
{
	m_nReadySceneID = nSceneID;
	m_nReadyLine    = nLineID;
}


void BattleFieldMgr::GoBackReadyScene(CRole *pRole)
{
	CSceneSwitch::RoleList roRoleList;
	roRoleList.push_back(pRole);
	KKSG::SceneSwitchData roData;
	CSceneSwitch::EnterScene(pRole, m_nReadySceneID, roData);
}


bool BattleFieldMgr::EnterReadyScene(CRole *pRole)
{
	// 检查活动是否开启
	if (OpenSystemMgr::Instance()->IsClosedSystem(SYS_BATTLEFIELD))
	{
		return false;
	}

	CSceneSwitch::RoleList roRoleList;
	roRoleList.push_back(pRole);
	KKSG::SceneSwitchData roData;
	bool bRet = CSceneSwitch::EnterScene(pRole, m_nReadySceneID, roData);
	return bRet;
}

void BattleFieldMgr::LoadTimer()
{
	time_t nowtime = GameTime::GetTime();
	UINT32 open_time = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_BATTLEFIELD);
	if ((UINT32)nowtime >= open_time)
	{
		return ;
	}
	auto info = GetGlobalConfig().BattleFieldPreNoticeList;
	for (auto it = info.begin(); it != info.end(); it ++)
	{
		if (open_time - nowtime > (*it)*60)
		{
			time_t pretime = open_time - (*it)*60;
			tm* sttime = localtime(&pretime);
			TimeSpecific notice_minute(sttime->tm_hour, sttime->tm_min, 0);
			UINT32 tmid= CTimeSpecificMgr::Instance()->RegisterEvent(notice_minute, std::bind(&BattleFieldMgr::PreNoticeStart, this, *it));
			timer_id_.insert(tmid);
		}
	}
}

void BattleFieldMgr::ClearTimer()
{
	for (auto it = timer_id_.begin(); it != timer_id_.end(); ++it)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*it);
	}
	timer_id_.clear();
}

bool BattleFieldMgr::PreNoticeStart(INT32 minute)
{
	Notice notice(BattleFieldPreNotice);		
	notice.Replace("{0}", ToString(minute));
	notice.Send();
	return true;
}

void BattleFieldMgr::SendIcon(CRole* pRole, UINT32 sysid, KKSG::HallIconState state)
{
	if (NULL == pRole)
	{
		return;
	}
	PtcM2C_HallIconMNtf ptc;
	ptc.m_Data.set_systemid(sysid);
	ptc.m_Data.set_state(state);
	pRole->Send(ptc);
}

void BattleFieldMgr::SendIconToAll(UINT32 sysid, KKSG::HallIconState state)
{
	for(auto iter = CRoleManager::Instance()->Begin(); iter != CRoleManager::Instance()->End(); iter++)
	{
		CRole* pRole = *iter;
		if (pRole->IsSystemOpened(sysid))
		{
			SendIcon(pRole, sysid, state);
		}
	}
}

void BattleFieldMgr::EnterScene(const KKSG::EnterBFFightBatchScene& data)
{
	for (int i = 0; i < data.scenes_size(); i++)
	{
		const KKSG::EnterBFFightScene &scene = data.scenes(i);
		for (int j = 0; j < scene.roles_size(); j++)
		{
			const KKSG::BattleFieldRoleInfo &role = scene.roles(j);
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(role.roleid());
			if (NULL!=pRole)
			{
				KKSG::SceneSwitchData info;
				CSceneSwitch::EnterScene(pRole, scene.sceneid(), info, scene.gsline(), true, scene.mapid());
				LogInfo("enter fm fight scene role=%llu sceneid=%d gsline=%d time=%llu",role.roleid(),scene.sceneid(),scene.gsline(),TimeUtil::GetMilliSecond());
			}
		}
	}
}

void BattleFieldMgr::SendReadySceneInfo(const KKSG::BattleFieldReadyScene &refInfo)
{
	PtcM2G_BattleFiedlReadySceneGsNtf ntf;
	ntf.m_Data.CopyFrom(refInfo);
	ntf.m_Data.set_param(m_nReadySceneID);
	GSLink::Instance()->SendToAllLine(ntf);
}


void BattleFieldMgr::SendMatchFail(const KKSG::BattleFailInfo &refInfo)
{
	PtcM2G_BattleFailInfoGsNtf ntf;
	ntf.m_Data.CopyFrom(refInfo);
	GSLink::Instance()->SendToAllLine(ntf);
}