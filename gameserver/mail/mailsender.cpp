#include "pch.h"
#include "mailsender.h"
#include "gamelogic/item.h"
#include "pb/project.pb.h"
#include "mail/ptcg2m_addmailgs.h"
#include "network/mslink.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "define/tlogenum.h"
#include "gamelogic/itemconfig.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "mail/mailconfig.h"
#include "servermgr/servermgr.h"
#include "idutil.h"

MailSender::MailSender()
:m_type(Mail_System)
,m_expiretime(MAIL_EXPIRE_TIME)
,m_reason(ItemFlow_Mail)
,m_subReason(ItemFlow_Mail_Give)
{
	
}

MailSender::~MailSender()
{

}

void MailSender::SetContent(UINT32 type, const std::string& title, const std::string& content)
{
	m_type = (MailType)(type);
	m_title = title;
	m_content = content;
}

void MailSender::SetExpiretime(UINT32 time)
{
	m_expiretime = time;
}

void MailSender::SetItems(std::vector<ItemDesc>& items)
{
	m_items = items;
}

void MailSender::SetReason(int reason, int subReason)
{
	m_reason = reason;
	m_subReason = subReason;
}
void MailSender::SetReason(int subReason)
{
	m_subReason = subReason;
}

void MailSender::AddItem(const ItemDesc& item)
{
	for(auto i = m_items.begin(); i != m_items.end(); i++)
	{
		if(item.itemID == (*i).itemID && item.FilterIsSame(*i))
		{
			(*i).itemCount += item.itemCount;
			return;
		}
	}
	m_items.push_back(item);
}

void MailSender::AddItem(const XItem* item)
{
	if(NULL == item) return;
	m_xitems.push_back(*item);
}

void MailSender::Send(UINT64 roleid)
{
	std::vector<UINT64> roleids;
	roleids.push_back(roleid);
	BroadCast(roleids);
}

void MailSender::BroadCast(std::vector<UINT64>& roleids)
{
	PtcG2M_AddMailGS ntf;
	KKSG::SMail* mail = ntf.m_Data.mutable_mail();
	ToKKSG(*mail);
	for (auto i = roleids.begin(); i != roleids.end(); ++i)
	{
		ntf.m_Data.add_roleids(*i);
	}

	if (GSConfig::Instance()->IsCrossGS())
	{
		std::set<UINT32> serverids;
		for (auto i = roleids.begin(); i != roleids.end(); ++i)
		{
			serverids.insert(ServerMgr::Instance()->GetFromServerIdByRoleId(*i));	
		}
		for (auto i = serverids.begin(); i != serverids.end(); ++i)
		{
			ServerMgr::Instance()->SendTo(*i, ntf);
		}
	}
	else
	{
		MSLink::Instance()->SendTo(ntf);
	}
}

void MailSender::SendGuild(UINT64 guildId)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		SSError << " func not for cross gs guildid = " << guildId << END;
		return;
	}
	CGuildSimple* pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(guildId);
	if (pGuild != NULL)
	{
		const auto& memVec = pGuild->getAllMember();
		for (auto i = memVec.begin(); i != memVec.end(); ++i)
		{
			Send(i->first);
		}
	}
}

void MailSender::AddItemExplain()
{
	bool isbegin = true;
	for(auto i = m_items.begin(); i != m_items.end(); i++)
	{
		ItemConf* pConf = ItemConfig::Instance()->GetItemConf((*i).itemID);
		if(NULL == pConf)
		{
			continue;
		}
		if(!isbegin)
		{	
			m_content += "{n}";
		}
		else isbegin = false;
		m_content += pConf->ItemName[0] + string("*") + ToString((*i).itemCount);
	}
	for(auto i = m_xitems.begin(); i != m_xitems.end(); i++)
	{
		ItemConf* pConf = ItemConfig::Instance()->GetItemConf((*i).itemID);
		if(NULL == pConf)
		{
			continue;
		}
		if(!isbegin)
		{	
			m_content += "{n}";
		}
		else isbegin = false;
		m_content += pConf->ItemName[0] + string("*") + ToString((*i).itemCount);
	}
}

void MailSender::AddConfigItem(const MailConf* content)
{
	if (content == NULL)
	{
		return;
	}
	for (size_t i = 0; i < content->m_rewards.size(); ++i)
	{
		ItemDesc item(content->m_rewards[i][0], content->m_rewards[i][1]);
		AddItem(item);
	}
}

void MailSender::ToKKSG(KKSG::SMail& mail)
{
	UINT32 timelen = m_expiretime;
	if(timelen > MAX_MAIL_EXPIRE_TIME)
	{
		timelen = MAX_MAIL_EXPIRE_TIME;
	}

	UINT32 timestamp = TimeUtil::GetTime();
	UINT32 timeexpire = timestamp + timelen;

	mail.set_uid(0);
	mail.set_type(m_type);
	mail.set_isread(false);
	mail.set_isdelete(false);
	mail.set_istemplate(false);
	mail.set_state(m_items.empty() && m_xitems.empty() ? Mail_None : Mail_Given);
	mail.set_timestamp(timestamp);
	mail.set_timeexpire(timeexpire);
	mail.set_title(m_title);
	mail.set_content(m_content);
	mail.set_reason(m_reason);
	mail.set_subreason(m_subReason);
	mail.set_extparam(m_extparam);

	for(auto i = m_items.begin(); i != m_items.end(); ++i)
	{
		(*i).ToKKSG(mail.add_items());
	}

	for(auto i = m_xitems.begin(); i != m_xitems.end(); ++i)
	{
		XItem::Item2KKSGItem(&(*i), mail.add_xitems());
	}
}

void MailSender::SetExtParam(const std::string& param)
{
	m_extparam = param;
}
