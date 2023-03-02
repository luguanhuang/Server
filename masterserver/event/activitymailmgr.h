#ifndef _ACTIVITY_MAIL_H
#define _ACTIVITY_MAIL_H
#include "table/AbyssPartyMail.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "define/globalconfigtodbdef.h"

struct RoleMail
{
	UINT64 nRole;
	MailData data;
};
class ActivityMailMgr
{
public:
	ActivityMailMgr();
	~ActivityMailMgr();
	DECLARE_SINGLETON(ActivityMailMgr)
public:
	bool Init();
	void Uninit();
	void ClearFile();
	bool LoadFile();
	bool OnTrigger(std::vector<RoleMail>& vecMail);
	void Clear();
	bool OnDayTimer();
	void Update();
	void OnSartUp();
	void CheckDuckUp();
	void SetMaxMail(UINT32 nMax);
	bool OnDayTimerDuck();
	void ClearDuck();
	void SendMail(UINT32 nTime, std::map<UINT32, std::vector<RoleMail>>& mailmap,std::vector<UINT32>& vecTimer);

private:
	AbyssPartyMail m_Mail;
	UINT32 m_timer;
	UINT32 m_maxMail;
	std::map<UINT32, std::vector<RoleMail>> m_mapMail;
	std::vector<UINT32> m_vecTimer;
	UINT32 m_ducktimer;
	std::map<UINT32, std::vector<RoleMail>> m_mapDuckMail;
	std::vector<UINT32> m_vecDuckTimer;
};

#endif