#include "pch.h"
#include "time.h"
#include "friend.h"
#include "frienddef.h"
#include "friendrecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "table/globalconfig.h"

#define RANDOM_FRIEND_CD ( GetGlobalConfig().FriendSystemRefreshAddListCD)
#define RANDOM_FRIEND_TIME 600

CFriendRecord::CFriendRecord()
{
	m_uniqueday = 0;
	m_lastquerytime = 0;
	Reset();
}

bool CFriendRecord::Load(const KKSG::FriendRecord& record)
{
	m_uniqueday = record.uniqueday();
	m_receive = record.receive();
	m_send = record.send();
	m_lastquerytime = record.lastquerytime();
	for (int i = 0; i < record.sends_size(); ++i)
	{
		m_sends.insert(record.sends(i));
	}
	Update();
	return true;
}

void CFriendRecord::Save(KKSG::FriendRecord& record)
{
	record.Clear();
	record.set_uniqueday(m_uniqueday);
	record.set_receive(m_receive);
	record.set_send(m_send);
	record.set_lastquerytime(m_lastquerytime);
	for (auto i = m_sends.begin(); i != m_sends.end(); ++i)
	{
		record.add_sends(*i);
	}
}	

void CFriendRecord::Reset()
{
	m_receive = 0;
	m_send = 0;
	m_sends.clear();
	m_ischanged = false;
}

void CFriendRecord::Update()
{
	UINT32 tempday = XCommon::GetTodayUnique();
	if (tempday != m_uniqueday)	
	{
		Reset();
		m_uniqueday = tempday;
	}
}

bool CFriendRecord::IsReceiveLimit()
{
	return m_receive >= MAX_GIFTRECEIVE;
}

bool CFriendRecord::IsSendLimit()
{
	return m_send >= MAX_GIFTSEND;
}

void CFriendRecord::AddReceive()
{
	++m_receive;
	m_ischanged = true;
}

bool CFriendRecord::AddSend(UINT64 roleid)
{
	if (m_sends.find(roleid) != m_sends.end())
	{
		return false;
	}
	++m_send;
	m_sends.insert(roleid);
	m_ischanged = true;
	return true;
}


UINT32 CFriendRecord::GetReceiveLeft()
{
	return m_receive >= MAX_GIFTRECEIVE ? 0 : MAX_GIFTRECEIVE - m_receive;
}

UINT32 CFriendRecord::GetSendLeft()
{
	return m_send >= MAX_GIFTSEND ? 0 : MAX_GIFTSEND - m_send;
}

bool CFriendRecord::UpdateQueryTime()
{
	UINT32 nowime = (UINT32)time(NULL);
	if (nowime >= m_lastquerytime + RANDOM_FRIEND_CD)
	{
		m_lastquerytime = nowime;
		return true;
	}
	return false;
}

void CFriendRecord::UpdateRandom(time_t nowtime)
{
	for (auto i = m_randomfriends.begin(); i != m_randomfriends.end();)	
	{
		if (nowtime > i->second + RANDOM_FRIEND_TIME)			
		{
			m_randomfriends.erase(i++);
		}
		else
		{
			++i;
		}
	}
}

void CFriendRecord::AddRandom(time_t nowtime, UINT64 id)
{
	m_randomfriends[id] = nowtime;
}

bool CFriendRecord::IsRandomValid(time_t nowtime, UINT64 id)
{
	return  m_randomfriends.end() == m_randomfriends.find(id);
}
