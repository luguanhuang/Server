#include "pch.h"
#include "chatcondition.h"
#include "time.h"
#include "pb/project.pb.h"


UINT32 LevelChatCondition::Check(ChatConditionParam& param)
{
	return param.level >= m_level ? KKSG::ERR_SUCCESS : KKSG::ERR_CHAT_LEVELLIMIT;
}

UINT32 LengthChatCondition::Check(ChatConditionParam& param)
{
	return param.length <= m_length ? KKSG::ERR_SUCCESS : KKSG::ERR_CHAT_LENGTHLIMIT;
}

UINT32 TimeChatCondition::Check(ChatConditionParam& param)
{
	UINT64 roleid = param.roleid;
	time_t nowtime = time(NULL);
	SSInfo << "Time Condition Check, RoleID:" << roleid << END;

	auto i = m_roleid2time.find(roleid);
	if (i == m_roleid2time.end())
	{
		m_roleid2time[roleid] = nowtime;
		SSInfo << "Not Find, Time Condition Check, RoleID:" << roleid << END;
		return KKSG::ERR_SUCCESS;
	}
	if (nowtime >= i->second + m_time)
	{
		m_roleid2time[roleid] = nowtime;
		SSInfo << "Time Pass By, Time Condition Check, RoleID:" << roleid << END;
		return KKSG::ERR_SUCCESS;
	}
	param.cooldown = (UINT32)(i->second + m_time - nowtime);
	SSInfo << "During CoolDown, Time Condition Check, RoleID:" << roleid << ", LeftTime:" << i->second + m_time - nowtime << ", m_time:" <<m_time << END;	
	return KKSG::ERR_CHAT_TIMELIMIT;
}

UINT32 BlackChatCondition::Check(ChatConditionParam& param)
{
	if (0 != param.destid)
	{
	}
	return KKSG::ERR_SUCCESS;
}