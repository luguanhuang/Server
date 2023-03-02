#include "pch.h"
#include "config.h"
#include "util.h"
#include "time.h"
#include "queuingmgr.h"
#include "account/accountsessionmgr.h"
#include "network/dblink.h"

INSTANCE_SINGLETON(QueuingMgr);

QueuingMgr::QueuingMgr()
{
	m_queuingtime = 0;
	m_readyexpiretime = 0;
	m_toqueuingcount = 0;
}

QueuingMgr::~QueuingMgr()
{

}

bool QueuingMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	CmdLine::RegistCmd("queue", CmdQueue);
	return true;
}

void QueuingMgr::Uninit()
{

}

bool QueuingMgr::LoadFile()
{
	std::string data = ControlConfig::Instance()->ReadFileContent("serveronly/queuing.txt");
	std::vector<std::string> Lines = Split(data, "\r\n");
	if (Lines.size() != 3)
	{
		return false;
	}

	UINT32 lastcount = m_toqueuingcount;

	std::vector<std::string> column1 = Split(Lines[0], ":");
	m_toqueuingcount = convert<UINT32>(column1[1]);
	std::vector<std::string> column2 = Split(Lines[1], ":");
	m_queuingtime = convert<UINT32>(column2[1]);
	std::vector<std::string> column3 = Split(Lines[2], ":");
	m_readyexpiretime = convert<UINT32>(column3[1]);

	if (0 != lastcount && !m_waitingqueue.empty())
	{
		for (; lastcount < m_toqueuingcount; ++lastcount)
		{
			if(m_waitingqueue.empty()) break;

			const WaitInfo& info = m_waitingqueue.front();
			auto i = m_id2wait.find(info.accountid);
			if (i != m_id2wait.end())
			{
				m_id2wait.erase(i);
			}
			else
			{
				SSWarn<<"not find in wait queue, accountid:"<<info.count<<END;
			}
			m_waitingqueue.pop_front();
		}
	}

	SSInfo<<"count:"<<m_toqueuingcount<<" time:"<<m_queuingtime<<END;

	return true;
}

void QueuingMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{

}

bool QueuingMgr::IsFull()
{
	UINT32 allacount = CAccountSessionMgr::Instance()->GetAllOnlineNum();
	UINT32 queuingaccount = m_waitingqueue.size();
	UINT32 inaccount = allacount > queuingaccount ? allacount - queuingaccount : 0;
	return inaccount >= m_toqueuingcount;
}

bool QueuingMgr::IsWaiting(const std::string& account)
{
	return m_id2wait.find(account) != m_id2wait.end();
}

//bool QueuingMgr::IsReady(const std::string& account)
//{
//	return m_id2ready.find(account) != m_id2ready.end();
//}

//void QueuingMgr::PopReady(const std::string& account)
//{
//	auto i = m_id2ready.find(account);
//	if (i != m_id2ready.end())
//	{
//		m_id2ready.erase(i);
//	}
//}

void QueuingMgr::PushWait(const std::string& account)
{
	if (IsWaiting(account))
	{
		SSInfo<<account<<" is in queue"<<END;
		return;
	}
	WaitInfo info;
	info.accountid = account;
	info.timestamp = (UINT32)time(NULL);
	info.count = m_waitingqueue.size() + 1;
	m_waitingqueue.push_back(info);
	m_id2wait[account] = info;
}

void QueuingMgr::PopWait(const std::string& account, UINT32 rolecount)
{
	///> 玩家在排队中退出
	auto i = m_id2wait.find(account);
	if (i != m_id2wait.end())
	{
		m_id2wait.erase(i);
		bool isfind = false;
		for (auto j = m_waitingqueue.begin(); j != m_waitingqueue.end();)
		{
			if (j->accountid == account)
			{
				j = m_waitingqueue.erase(j);
				isfind = true;
			}
			else
			{
				if (isfind)
				{
					///> 排在他后面的人都进1
					if (j->count != 0)
					{
						j->count -= 1;
					}
					else
					{
						SSWarn<<"queuing account:"<<account<<" count=0"<<END;
					}
				}
				++j;
			}
		}
		SSInfo<<account<<" quit from wait queue"<<END;
	}
	else
	{
		///> 玩家在游戏中退出
		if (m_waitingqueue.empty())
		{
			return;
		}
		if (IsFull())
		{
			SSInfo<<"is full, can't login"<<END;
			return;
		}
		WaitInfo info = m_waitingqueue.front();
		info.timestamp = (UINT32)time(NULL) + m_readyexpiretime;
		//m_id2ready[info.accountid] = info;

		m_waitingqueue.pop_front();
		auto i = m_id2wait.find(info.accountid);
		if (i != m_id2wait.end())
		{
			m_id2wait.erase(i);
		}
		SSInfo<<account<<" logout, "<<info.accountid<<" is ready"<<END;
	}
}

const WaitInfo* QueuingMgr::GetWaitInfo(const std::string& account)
{
	auto i = m_id2wait.find(account);
	if (i == m_id2wait.end())
	{
		return NULL;
	}
	return &i->second;
}

UINT32 QueuingMgr::GetWaitPreCount(const std::string& accountid)
{
	UINT32 count = 1;
	for (auto i = m_waitingqueue.begin(); i != m_waitingqueue.end(); ++i)
	{
		if (i->accountid == accountid)
		{
			break;
		}
		++count;
	}
	return count;
}

void QueuingMgr::GetWaitRes(const WaitInfo* info, WaitRes& res)
{
	UINT32 count = GetWaitPreCount(info->accountid);
	res.rolecount = count;
	UINT32 timeleft = count * m_queuingtime;
	res.timeleft = timeleft > 7200 ? 7200 : timeleft;
}		

bool QueuingMgr::CmdQueue(const CmdLineArgs &args, std::string &outputMessage)
{
	return QueuingMgr::Instance()->LoadFile();
}
