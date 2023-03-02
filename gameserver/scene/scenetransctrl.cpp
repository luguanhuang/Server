#include "pch.h"
#include "scenetransctrl.h"
#include "unit/role.h"
#include "util/gametime.h"
#include "util/XRandom.h"
#include "scene/scene.h"
#include "buff/buffrecord.h"

SceneTransCtrl::SceneTransCtrl(Scene* scene, TransConfig* config)
{
	m_scene = scene;
	m_config = config;
	m_timestamp = 0;
	m_isin = false;
	OnTick(true);
}

void SceneTransCtrl::OnEnter(Role* role)
{
	if (!IsInTime())
	{
		return;
	}
	// trans
	if (0 != m_buff.buffID)
	{
		role->Get<BuffRecord>()->AddTransBuff(m_buff.buffID, m_buff.BuffLevel, 0);
	}
}

bool SceneTransCtrl::OnLeave(Role* role)
{
	if (!IsInTime(NULL == m_config ? 0 : m_config->interval))
	{
		return false;
	}
	// remove
	role->Get<BuffRecord>()->DelTransBuff(3);
	return true;
}

void SceneTransCtrl::OnTick(bool isinit)
{
	if (!IsInTime())
	{
		m_isin = false;
		return;
	}
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	if (!m_isin)
	{
		///> 第一进入时，随机时间
		UINT32 random = XRandom::randInt(0, 10);
		m_timestamp = nowtime + random;
		m_isin = true;
	}
	if (nowtime > m_timestamp)
	{
		m_buff = RandomBuff();
		if (!isinit && 0 != m_buff.buffID)
		{
			//add buff
			const std::list<Role*>& allroles = m_scene->GetAllRoles();
			for (auto i = allroles.begin(); i != allroles.end(); ++i)
			{
				Role* role = *i;
				role->Get<BuffRecord>()->AddTransBuff(m_buff.buffID, m_buff.BuffLevel, 0);
			}
		}
		UINT32 random = XRandom::randInt(0, 10);
		m_timestamp = nowtime + ((NULL == m_config) ? 300 : m_config->interval) + random;
	}
}

bool SceneTransCtrl::IsInTime(UINT32 offset)
{
	if (NULL == m_config)
	{
		return false;
	}
	if ((UINT32)(-1) == offset)
	{
		offset = 0;
	}
	UINT32 nowtime = (UINT32)GameTime::GetTime();
	if (nowtime >= m_config->timespan[0] && nowtime <= m_config->timespan[1] + offset)
	{
		return true;
	}
	else
	{
		return false;
	}
}

BuffDesc SceneTransCtrl::RandomBuff()
{
	BuffDesc buff;
	UINT32 all = 0;
	for (auto i = m_config->randombuff.begin(); i != m_config->randombuff.end(); ++i)
	{
		all += i->seq[0];
	}
	UINT32 random = XRandom::randInt(0, all + 1);
	all = 0;
	for (auto i = m_config->randombuff.begin(); i != m_config->randombuff.end(); ++i)
	{
		all += i->seq[0];
		if (random <= all)
		{
			buff.buffID = i->seq[1];
			buff.BuffLevel = i->seq[2];
			break;
		}
	}
	return buff;
}
