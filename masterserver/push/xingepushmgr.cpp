#include "pch.h"
#include "xingepushmgr.h"
#include "httptask/multipushtask.h"
#include "timeutil.h"
#include "httptask/httptaskmgr.h"


XinGePushMgr::~XinGePushMgr()
{
	for (auto it = m_multipush.begin(); it != m_multipush.end(); it++)
	{
		delete it->second;
	}
}

void XinGePushMgr::StartTimer()
{
	if (m_timer)
	{
		return;
	}
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 50, -1, __FILE__, __LINE__);	
}

void XinGePushMgr::EndTimer()
{
	if (!m_timer)
	{
		return;
	}
	CTimerMgr::Instance()->KillTimer(m_timer);
	m_timer = 0;
}

void XinGePushMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (m_multipush.empty())
	{
		EndTimer();
		return;
	}
	Update();
}

void XinGePushMgr::Update()
{
	UINT32 now = TimeUtil::GetTime();
	for (auto it = m_multipush.begin(); it != m_multipush.end(); )
	{
		XinGeMultiPushItem* item = it->second;
		// 推送结束
		if (item->roles.empty())
		{
			SSInfo << "Finish Push, pushId: " << item->pushId << ", pushNum: " << item->pushNum 
				     << ", Use time: " << (now - item->startTime) << END;
			delete item;
			m_multipush.erase(it++);
			continue;
		}

		if (item->pushId == 0)
		{
			// 创建推送超时
			if (now > item->startTime + 10)
			{
				SSError << "Create Push TimeOut" << END;
				delete item;
				m_multipush.erase(it++);
				continue;
			}
			else
			{
				it++;
				continue;
			}
		}

		MultiPushTask* task = new MultiPushTask(item->pushId);
		task->SetPushType(item->pushType);
		task->SetPlatType(item->platType);
		task->PackTokens(item->roles);
		HttpTaskMgr::Instance()->AddTask(task);
		it++;
	}
}

void XinGePushMgr::AddMultiPush(XinGeMultiPushItem* item)
{
	item->id = ++m_curId;
	item->startTime = TimeUtil::GetTime();
	item->pushNum = item->roles.size();

	m_multipush.insert(std::make_pair(item->id, item));

	CreateMultiPushTask* task = new CreateMultiPushTask(item->id, this);
	task->SetPushType(item->pushType);
	task->SetPlatType(item->platType);
	PushMessage& message = task->GetMessage();
	message.title = item->title;
	message.content = item->content;

	HttpTaskMgr::Instance()->AddTask(task);
	StartTimer();
}

void XinGePushMgr::OnCreatePushFailed(UINT32 id)
{
	SSError << "Push Error, id: " << id << END;
	auto it = m_multipush.find(id);
	if (it == m_multipush.end())
	{
		return;
	}
	delete it->second;
	m_multipush.erase(it);
}

void XinGePushMgr::OnCreatePushSuccess(UINT32 id, UINT32 pushId)
{
	auto it = m_multipush.find(id);
	if (it == m_multipush.end())
	{
		return;
	}
	it->second->pushId = pushId;
}
