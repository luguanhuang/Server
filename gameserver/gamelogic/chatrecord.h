#ifndef __CHAT_RECORD_H__
#define __CHAT_RECORD_H__

#include "unit/roleserilizeinterface.h"
#include <map>
#include "unit/role.h"

class ChatRecord : public RoleSerializeInterface
{
public:
	ChatRecord(Role* role);
	virtual ~ChatRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void InitChat();
	void UpdateChatTimes();
	void DayZero();
	void OnDayPass();

	void SetLastChatTime(UINT32 channel);
	UINT32 GetLastChatTime(UINT32 channel);	

public:
	Role* m_pRole;
	Switch m_ismodify;
	std::map<UINT32, UINT32> m_lastChatTime;
	UINT32 m_lastUpdateTime;
	UINT32 m_dwWorldChatTimes;
};

#endif // __LOTTERYRECORD_H__