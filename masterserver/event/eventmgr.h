#ifndef __DESINATIONMGR_H
#define __DESINATIONMGR_H
//数据库存储字段blob 是64k 这边每个结构体20byte
//keyroleid =====>{value==updatetime bupdate, set<stEvent>} 设计的时候第一确保数据的准确性 二确保不要老是让mysql 空跑
#define MAX_EVENT_SIZE			3200
#define EVENT_REFRESH_TIME		5
#include "define/eventdef.h"

class CRole;
class Guild;

enum EventState
{
	EVENT_INVALID				= 0,
	EVENT_ADDED					= 1,
	EVENT_SENDED				= 2,
	EVENT_ACKSUCCESS			= 3,
	EVENT_ACKERROR				= 4,
};



class EventMgr
{
	EventMgr();
	~EventMgr();
	DECLARE_SINGLETON(EventMgr)
public:
	struct stAllEvent
	{
		stAllEvent()
		{
			bUpdate = false;
		}
		bool bUpdate;
		std::map<UINT64, GlobalEvent::stEvent> mapAllEvent;//KEY　eventid
	};
	bool Init();
	void Uninit();
	void Update();
	void DelEvent(UINT64 nRoleID,UINT64 nEventID);
	void DelEvent(UINT64 nRoleID,std::vector<UINT64>& vecEventIDs);
	void OnChangedGS(CRole* pRole);
	void AddEvent(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1=0,UINT32 nParam2=0,UINT32 nParam3=0,UINT64 lParam1=0,UINT64 lParam2=0, const std::string& szName = std::string());
	void AddEvent(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1,UINT32 nOper,std::string szName);
	void AddDes(UINT64 nRoleID,UINT32 nDesID);
	void DelDes(UINT64 nRoleID,UINT32 nDesID);
	void AddAch(UINT64 nRoleID,UINT32 nDesID);
	void DelAch(UINT64 nRoleID,UINT32 nDesID);
	void AddEventOnlyOnMS(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1=0,UINT32 nParam2=0,UINT32 nParam3=0,UINT64 lParam1=0,UINT64 lParam2=0);
	void AddEventFromGS(UINT64 nRoleID,UINT32 nEventID,UINT32 nParam1=0,UINT32 nParam2=0,UINT32 nParam3=0,UINT64 lParam1=0,UINT64 lParam2=0);
	void AddGuildEvent(Guild *pGuild,UINT32 nEventID,UINT32 nParam1=0,UINT32 nParam2=0,UINT32 nParam3=0,UINT64 lParam1=0,UINT64 lParam2=0);
	void TestSave();
	UINT64 NewEventID(UINT32 nEventID);
	UINT32 GetSize();
private:
	void _AddEvent(UINT64 nRoleID,GlobalEvent::stEvent *pEvent);
	void AddOneEvent(UINT64 nRoleID,GlobalEvent::stEvent *pEvent);
	void SendOneEvent(CRole* pRole,UINT64 nUUID,GlobalEvent::stEvent *pEvent);
	void _SendEvent(CRole* pRole);
	void _UpdateToDB(bool isAll = false);
	bool _ReadAllRoleEventFromDB();
	bool _IsBonusEvent(UINT32 eventID);
	bool _IsMentorEvent(UINT32 eventID);

private:
	std::map<UINT64, stAllEvent> mapEvent;
	UINT32 m_nTime; 
	UINT64 m_nIndex;
	UINT32 m_nMaxIndex;
};



#endif