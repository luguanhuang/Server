#ifndef __MAIL_DATA_H__
#define __MAIL_DATA_H__

#include "define/itemdef.h"
#include "define/maildef.h"
#include "define/tlogenum.h"

namespace KKSG
{
	class SMail;
}

struct MailData
{
	MailType		m_type;
	UINT32			m_expiretime;
	int				m_reason;
	int				m_subReason;
	std::string		m_title;
	std::string		m_content;
	std::vector<ItemDesc>	m_items;

	int m_latesend;		///> 策划需求邮件不要立即给，延迟给

	std::string m_extparam;//扩展参数，邮件需要支持扩展参数可用该字段;
	MailData()
		:m_type(Mail_System)
		,m_expiretime(0)
		,m_reason(ItemFlow_Mail)
		,m_subReason(ItemFlow_Mail_Give)
		,m_latesend(0)
	{
	}
	MailData(const std::string& title, const std::string& content, const std::vector<ItemDesc>& item,UINT32 expiretime)
	{
		m_type = Mail_System;
		m_title = title;
		m_content = content;
		m_items = item;
		m_expiretime = expiretime;
		m_latesend = 0;
		m_reason = ItemFlow_Mail;
		m_subReason = ItemFlow_Mail_Give;
	}
	~MailData()
	{
	}

	void ToKKSG(KKSG::SMail& roMail) const;
	void SetReason(int reason, int subReason);
	void SetReason(int subReason);
	void SetExtParam(const std::string& param);
};


UINT64 NewMailID();


#endif