#include "pch.h"
#include <time.h>
#include "chatrecord.h"
#include "pb/project.pb.h"
#include "util/XRandom.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "payv2Record.h"
#include "chatmgr.h"

ChatRecord::ChatRecord(Role* role)
{
	m_pRole = role;
	m_lastUpdateTime = 0; 
	m_dwWorldChatTimes = 0; 
}

ChatRecord::~ChatRecord()
{

}

bool ChatRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	m_lastUpdateTime = poRoleAllInfo->chat().lastupdatetime();
	m_dwWorldChatTimes = poRoleAllInfo->chat().worldchattimes(); 	
	if(!XCommon::IsDaySame(m_lastUpdateTime, TimeUtil::GetTime()))
	{
		OnDayPass();
	}
	return true;
}

void ChatRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if(!m_ismodify.TestAndReset())
	{
		return; 
	}
	poRoleAllInfo->mutable_chat()->set_lastupdatetime(m_lastUpdateTime);
	poRoleAllInfo->mutable_chat()->set_worldchattimes(m_dwWorldChatTimes);

	roChanged.insert(poRoleAllInfo->mutable_chat());
}

void ChatRecord::InitChat()
{
	m_dwWorldChatTimes = 0; 
	m_lastUpdateTime = 0; 
	m_ismodify.Set();
}

void ChatRecord::UpdateChatTimes()
{
	if (m_pRole->Get<CPayV2Record>()->GetRemainChatCount())
	{
		m_pRole->Get<CPayV2Record>()->UpdateChatCount();
	}
	else
	{
		m_dwWorldChatTimes += 1;
	}
	m_ismodify.Set();
	SSInfo << "update chat times, world chat times: " << m_dwWorldChatTimes + m_pRole->Get<CPayV2Record>()->GetRemainChatCount() << END;
}

void ChatRecord::DayZero()
{
	m_lastUpdateTime = TimeUtil::GetTime(); 
	m_dwWorldChatTimes = 0;
	m_ismodify.Set();
}

void ChatRecord::OnDayPass()
{
	DayZero();
	ChatMgr::Instance()->SendWorldChannelLeftTimesNotify(m_pRole);
	m_pRole->Get<CPayV2Record>()->ClearUpdateChatCount();
}

void ChatRecord::SetLastChatTime(UINT32 channel)
{
	if (KKSG::ChatChannel_IsValid(channel))
	{
		m_lastChatTime[channel] = TimeUtil::GetTime();
	}
}

UINT32 ChatRecord::GetLastChatTime(UINT32 channel)
{
	if (KKSG::ChatChannel_IsValid(channel))
	{
		auto iter = m_lastChatTime.find(channel);
		if (iter == m_lastChatTime.end())
		{
			m_lastChatTime.insert(std::make_pair(channel, 0));
			return 0;
		}
		return iter->second;
	}
	return 0;
}