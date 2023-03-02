#include "pch.h"
#include "maildata.h"
#include "timeutil.h"
#include "pb/project.pb.h"
#include "config.h"


UINT64 NewMailID()
{
	static UINT16 sIndex = 0;
	UINT32 dwLow = MakeUINT32((UINT16)MSConfig::Instance()->GetServerID(), sIndex++);
	return MakeUINT64(TimeUtil::GetTime(), dwLow);
}

void MailData::ToKKSG(KKSG::SMail& roMail) const
{
	UINT32 timelen = m_expiretime;
	if(timelen > MAX_MAIL_EXPIRE_TIME)
	{
		timelen = MAX_MAIL_EXPIRE_TIME;
	}

	UINT32 timestamp = TimeUtil::GetTime();
	UINT32 timeexpire = timestamp + timelen;

	roMail.set_uid(NewMailID());
	roMail.set_type(m_type);
	roMail.set_isread(false);
	roMail.set_isdelete(false);
	roMail.set_istemplate(false);
	roMail.set_state(m_items.empty() ? Mail_None : Mail_Given);
	roMail.set_timestamp(timestamp);
	roMail.set_timeexpire(timeexpire);
	roMail.set_title(m_title);
	roMail.set_content(m_content);
	roMail.set_reason(m_reason);
	roMail.set_subreason(m_subReason);
	roMail.set_extparam(m_extparam);
	
	for(auto i = m_items.begin(); i != m_items.end(); ++i)
	{
		(*i).ToKKSG(roMail.add_items());
	}
}

void MailData::SetReason(int reason, int subReason)
{
	m_reason = reason;
	m_subReason = subReason;
}
void MailData::SetReason(int subReason)
{
	m_subReason = subReason;
}


void MailData::SetExtParam(const std::string& param)
{
	m_extparam = param;
}
