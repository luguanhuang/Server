#ifndef __DESINATIONMGR_H
#define __DESINATIONMGR_H
//数据库存储字段blob 是64k 这边每个结构体20byte
//keyroleid =====>{value==updatetime bupdate, set<stEvent>} 设计的时候第一确保数据的准确性 二确保不要老是让mysql 空跑
#define MAX_EVENT_SIZE			3200
#define EVENT_REFRESH_TIME		5
#include "define/eventdef.h"

class Role;
class Guild;

class EventMgr
{
	EventMgr();
	~EventMgr();
	DECLARE_SINGLETON(EventMgr)
public:
	struct stAllEvent
	{
		bool bUpdate;
		std::map<UINT64, GlobalEvent::stEvent> mapAllEvent;//KEY　eventid
	};
	bool Init();
	void Uninit();
	void Update();
	void DelEvent(UINT64 nRoleID,UINT64 nEventID);
	void Clear(UINT64 nRoleID);
	void AddEvent(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1=0,UINT32 nParam2=0,UINT32 nParam3=0,UINT64 lParam1=0,UINT64 lParam2=0);
	void AddEventOnlyOnGS(Role* role,UINT32 nEventID,UINT32 nParam1=0,UINT32 nParam2=0,UINT32 nParam3=0,UINT64 lParam1=0,UINT64 lParam2=0);
	void AddGuildBonusEvent(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1=0,UINT32 nParam2=0,UINT32 nParam3=0,UINT64 lParam1=0,UINT64 lParam2=0);
	void TestSave();
	UINT64 NewEventID(UINT32 nEventID);
	void OnConnectMS(UINT32 nServerID=0);
private:
	void _AddEvent(UINT64 nRoleID,GlobalEvent::stEvent *pEvent);
	std::map<UINT64, stAllEvent> mapEvent;
	UINT32 m_nTime; 
	UINT64 m_nIndex;
	UINT32 m_nMaxIndex;
};

#endif