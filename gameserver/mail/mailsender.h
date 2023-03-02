#ifndef __MAILSENDER_H__
#define __MAILSENDER_H__

#include "define/maildef.h"

namespace KKSG 
{
	class SMail;
};

struct ItemDesc;
struct XItem;
struct MailConf;
class MailSender
{
public:
	MailSender();
	~MailSender();

	void SetExpiretime(UINT32 time);
	void SetContent(UINT32 type, const std::string& title, const std::string& content);
	void SetItems(std::vector<ItemDesc>& items);
	void SetReason(int reason, int subReason);
	void SetReason(int subReason);

	void AddItem(const ItemDesc& item);
	void AddItem(const XItem* item);

	void Send(UINT64 roleid);

	void BroadCast(std::vector<UINT64>& roleids);
	void SendGuild(UINT64 guildId);

	void AddItemExplain();
	void AddConfigItem(const MailConf* content);

	void SetExtParam(const std::string& param);
private:
	void ToKKSG(KKSG::SMail& mail);

private:
	MailType		m_type;
	UINT32			m_expiretime;
	int				m_reason;
	int				m_subReason;
	std::string		m_title;
	std::string		m_content;
	std::vector<ItemDesc>	m_items;
	std::vector<XItem>		m_xitems;
	std::string m_extparam;//扩展参数，邮件需要支持扩展参数可用该字段;
};

#endif