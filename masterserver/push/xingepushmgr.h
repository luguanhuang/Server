#ifndef _XINGE_PUSH_MGR_H_
#define _XINGE_PUSH_MGR_H_

#include <map>
#include <string>
#include <list>
#include "timer.h"

class CRoleSummary;

struct XinGeMultiPushItem
{
	XinGeMultiPushItem():
		id(0), pushType(0), platType(0), startTime(0), pushId(0), pushNum(0)
	{
	}

	UINT32 id;
	UINT32 pushType;
	UINT32 platType;
	std::string title;	
	std::string content;

	std::list<CRoleSummary*> roles;

	UINT32 startTime;
	UINT32 pushId;
	UINT32 pushNum;
};

// 信鸽推送管理批量推送
class XinGePushMgr : public ITimer
{
public:
	XinGePushMgr() :m_curId(0), m_timer(0)
	{
	}
	~XinGePushMgr();

	void Update();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void AddMultiPush(XinGeMultiPushItem* item);
	void OnCreatePushFailed(UINT32 id);
	void OnCreatePushSuccess(UINT32 id, UINT32 pushId);

private:
	void StartTimer();
	void EndTimer();

private:
	UINT32 m_curId;
	HTIMER m_timer;
	std::map<UINT32, XinGeMultiPushItem*> m_multipush;	// 批量推送信息
};

#endif