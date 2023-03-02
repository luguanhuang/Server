#include "pch.h"
#include "QAMgr.h"
#include "pb/project.pb.h"
#include "item.h"
#include "qa/ptcg2c_qanotify.h"
#include "unit/role.h"
#include "timeutil.h"
#include "table/EquipSuitTable.h"
#include "table/QALevelRewardTable.h"
#include "QARecord.h"
#include "unit/rolemanager.h"
#include "util/XRandom.h"
#include "designationRecord.h"
#include "designationMgr.h"
#include "bagtransition.h"
#include "define/tlogenum.h"
#include "util/XCommon.h"
#include "mail/mailsender.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "rolesummary.h"
#include "rolesummarymgr.h"
#include "noticemgr.h"
#include "MultActivityMgr.h"
#include "utility/tlogger.h"
#include "qa/ptcg2m_gmcloseqaroom.h"
#include "util/gametime.h"


INSTANCE_SINGLETON(QAMgr);

QAMgr::QAMgr()
:m_timerId(0)
,m_lastUpdateTime(0)
{

}

QAMgr::~QAMgr()
{

}

bool QAMgr::Init()
{

	m_timerId = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	LoadFile();

	return true;
}

void QAMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timerId);
	ClearFile();
}

bool QAMgr::CheckFile()
{
	QAConditionTable tmpQAConditionTable;
	if( !tmpQAConditionTable.LoadFile("table/QACondition.txt") )
	{
		SSError<<"check file QACondition.txt failed"<<END;
		return false;
	}
	return true;
}

bool QAMgr::LoadFile()
{
	bool bRet = CheckFile();
	if (bRet==false)
	{
		return false;
	}
	ClearFile();
	if( !m_QAConditionTable.LoadFile("table/QACondition.txt") )
	{
		SSError<<"load file QACondition.txt failed"<<END;
		return false; 
	}
	return true;
}

void QAMgr::ClearFile()
{
	m_QAConditionTable.Clear();
}


void QAMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Update();
}

void QAMgr::Update()
{
	UINT32 now = GameTime::GetTime();
	// 全民答题准备时间已过, 通知没参加的人图标消失
	UINT32 prepareEndTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_MULTI_QA) + GetPrepareTime(QATYPE_MULTI);
	if (m_lastUpdateTime < prepareEndTime && now >= prepareEndTime)
	{
		PushQAOverNotifyToAllNotEnter();
	}
	m_lastUpdateTime = now;
}

void QAMgr::SetGuildRoomId(UINT64 guildId, UINT32 roomId)
{
	m_guildRooms[guildId] = roomId;
	m_guildLastOpenTime[guildId] = GameTime::GetTime();
}

UINT32 QAMgr::GetGuildLastOpenTime(UINT64 guildId)
{
	auto it = m_guildLastOpenTime.find(guildId);
	if (it != m_guildLastOpenTime.end())
	{
		return it->second;
	}
	return 0;
}

void QAMgr::RemoveGuildRoom(UINT64 guildId)
{
	m_guildRooms.erase(guildId);
}

bool QAMgr::HasGuildRoom(UINT64 guildId)
{
	return m_guildRooms.find(guildId) != m_guildRooms.end();
}

bool QAMgr::IsLevelOk(QAConditionTable::RowData* row, UINT32 level)
{
	for (size_t i=0; i < row->LevelSection.size(); i++ )
	{
		if (row->LevelSection[i][0] <= level && level <= row->LevelSection[i][1])
		{
			return true;
		}
	}
	return false;
}

bool QAMgr::IsTimeOk(UINT32 type, UINT32 now)
{
	if (type == QATYPE_ONE)
	{
		return true;
	}

	UINT32 actId = MULT_ACTIVITY_GUILD_QA;
	if (type == QATYPE_MULTI)
	{
		actId = MULT_ACTIVITY_MULTI_QA;
	}
	UINT32 leftTime;
	MultActivityTime openTime;
	KKSG::MulActivityTimeState eState = MultActivityMgr::Instance()->GetOpenTimeState(actId, leftTime, openTime);
	if (eState != KKSG::MULACTIVITY_RUNNING)
	{
		return false;
	}

	// 多人活动超过准备时间之后不能新开房间
	if (type == QATYPE_MULTI)
	{
		UINT32 todaySec = now - (UINT32)XCommon::GetTodayBeginTime(false);
		UINT32 prepareTime = GetPrepareTime(type);
		if (todaySec > (openTime.startSec + prepareTime))
		{
			return false;
		}
	}
	return true;
}

UINT32 QAMgr::GetPrepareTime(UINT32 type)
{
	QAConditionTable::RowData* pData = m_QAConditionTable.GetByQAType(type);
	if (!pData)
	{
		return 0;
	}
	return pData->PrepareTime;
}

KKSG::ErrorCode QAMgr::CanRoleEnter(Role* role, UINT32 type)
{
	QARecord* record = role->Get<QARecord>();

	QAConditionTable::RowData* pData = m_QAConditionTable.GetByQAType(type);
	if(!pData)
	{
		return KKSG::ERR_QA_NO_DATA;
	}

	// 等级判断
	if(!IsLevelOk(pData, role->GetLevel()))
	{
		return KKSG::ERR_QA_LEVEL_NOT_ENOUGH;
	}

	// 时间判断 (notice:多人活动过了准备时间不能进入)
	if (!IsTimeOk(type, GameTime::GetTime()))
	{
		return KKSG::ERR_QA_NOT_IN_TIME;
	}

	// 公会判断
	if (type == QATYPE_GUILD)
	{
		CGuildSimple* pGuild = GuildSimpleMgr::Instance()->GetGuildByRoleId(role->GetID());
		if (!pGuild)
		{
			return KKSG::ERR_QA_NO_GUILD;
		}
	}

	// 个人次数判断
	UINT32 useNum = record->GetQACount(type);
	UINT32 cfgNum = pData->Count;
	if (useNum >= cfgNum)
	{
		return KKSG::ERR_QA_NO_COUNT;
	}
	return KKSG::ERR_SUCCESS;
}

void QAMgr::OnRoleLogin(Role* role)
{
	QARecord* record = role->Get<QARecord>();
	if (record->IsPlayingQA())
	{
		PushQANotify(role, record->GetCurType(), true);
		return;
	}

	KKSG::MulActivityTimeState eState = MultActivityMgr::Instance()->GetOpenTimeState(MULT_ACTIVITY_MULTI_QA);
	if (eState == KKSG::MULACTIVITY_RUNNING)
	{
		TriggerQA(role, QATYPE_MULTI);
		return;
	}

	eState = MultActivityMgr::Instance()->GetOpenTimeState(MULT_ACTIVITY_GUILD_QA);
	if (eState == KKSG::MULACTIVITY_RUNNING)
	{
		TriggerQA(role, QATYPE_GUILD);
		return;
	}
}

void QAMgr::TriggerQA(Role* role, UINT32 type)
{
	KKSG::ErrorCode code = CanRoleEnter(role, type);
	if (code != KKSG::ERR_SUCCESS)
	{
		return;
	}

	QAConditionTable::RowData* pData = m_QAConditionTable.GetByQAType(type);
	if(!pData)
	{
		return ;
	}
	UINT32 now = GameTime::GetTime();
	QARecord* record = role->Get<QARecord>();
	UINT32 lastTrigTime = record->GetTriggerTime(type);
	if (lastTrigTime + pData->CoolDown > now)
	{
		return; 
	}

	if (QATYPE_GUILD == type && !HasGuildRoom(role->getGuildId()))
	{
		return;
	}

	PushQANotify(role, type);
}

void QAMgr::PushQANotify(Role* role, UINT32 type, bool playing)
{
	PtcG2C_QANotify ntf;
	ntf.m_Data.set_type(type);
	ntf.m_Data.set_is_playing(playing);
	ntf.m_Data.set_is_over(false);
	role->Send(ntf);
}

void QAMgr::PushQAOverNotifyToAllNotEnter()
{
	PtcG2C_QANotify ntf;
	ntf.m_Data.set_type(QATYPE_MULTI);
	ntf.m_Data.set_is_playing(false);
	ntf.m_Data.set_is_over(true);

	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* pRole = *i;
		if(!pRole) 
		{
			continue;
		}
		QARecord* record = pRole->Get<QARecord>();
		if (record->IsPlayingQA())
		{
			continue;
		}
		pRole->Send(ntf);
	}
}


bool QAMgr::IsGuildQAOpen()
{
	MultActivityConf* pConf = MultActivityMgr::Instance()->GetActivityConfig(MULT_ACTIVITY_GUILD_QA);
	if (pConf==NULL)
		return false;
	UINT32 nDay = GameTime::GetWeekDay(GameTime::GetTime());
	for(auto iter = pConf->openDays.begin(); iter!= pConf->openDays.end(); iter++)
	{
		if (nDay == *iter)
		{
			return true;
		}
	}
	return false;
}

void QAMgr::NotifyGuildRoomOpen(UINT64 guild)
{
	CGuildSimple* pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(guild);
	if (!pGuild)
	{
		return;
	}
	const std::map<UINT64, UINT32>& gmems = pGuild->getAllMember();
	for (auto it = gmems.begin(); it != gmems.end(); ++it)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(it->first);
		if (role && CanRoleEnter(role, QATYPE_GUILD) == KKSG::ERR_SUCCESS)
		{
			PushQANotify(role, QATYPE_GUILD);
		}
	}
}

void QAMgr::NotifyGuildRoomClose(UINT64 guildId)
{
	CGuildSimple* pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(guildId);
	if (!pGuild)
	{
		return;
	}

	PtcG2C_QANotify ntf;
	ntf.m_Data.set_type(QATYPE_GUILD);
	ntf.m_Data.set_is_playing(false);
	ntf.m_Data.set_is_over(true);

	const std::map<UINT64, UINT32>& gmems = pGuild->getAllMember();
	for (auto it = gmems.begin(); it != gmems.end(); ++it)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(it->first);
		if (role)
		{
			role->Send(ntf);
		}
	}
}

void QAMgr::GMOpenQA(Role* role, UINT32 type)
{
	UINT32 now = GameTime::GetTime();
	UINT32 zero = XCommon::GetTodayBeginTime(false);
	UINT32 startTime = now + 2 - zero;
	UINT32 endTime = now + 2 + 60 * 15 - zero;

	if (type == QATYPE_ONE)
	{
		TriggerQA(role, type);
	}
	else if (type == QATYPE_GUILD)
	{
		MultActivityMgr::Instance()->GMAddOpenDayTime(MULT_ACTIVITY_GUILD_QA, startTime, endTime);
	}
	else if (type == QATYPE_MULTI)
	{
		MultActivityMgr::Instance()->GMAddOpenDayTime(MULT_ACTIVITY_MULTI_QA, startTime, endTime);
	}
	SSInfo << "GM open type: " << type << END;
}	

void QAMgr::GMResetRole(Role* role)
{
	PtcG2M_GMCloseQARoom ntf;
	ntf.m_sessionID = role->GetSessionID();
	role->SendToMS(ntf);

	SSWarn << "GM Rest Role: " << role->GetID() << END;
	QARecord* record = role->Get<QARecord>();
	record->OnDayPass(true);

	TriggerQA(role, QATYPE_MULTI);
	TriggerQA(role, QATYPE_GUILD);
}
