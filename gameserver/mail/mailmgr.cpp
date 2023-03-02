#include "pch.h"
//#include "mailmgr.h"
//#include "dblink.h"
//#include "time.h"
//#include "maildef.h"
//#include "unit/role.h"
//#include "unit/rolemanager.h"
//#include "gamelogic/item.h"
//#include "mail/mailop.h"
//#include "utility/uidhelper.h"
//#include "mail/mailrecord.h"
//#include "gamelogic/systemiddef.h"
//#include "common/ptcg2c_hintnotify.h"
//#include "mail/ptcg2d_addmailtemplatereport.h"
//#include "mail/ptcg2d_querymailtemplatereport.h"
//
//INSTANCE_SINGLETON(MailMgr);
//
//bool MailTemplate::IsExpire()
//{
//	UINT32 nowtime = (UINT32)time(NULL);
//	return nowtime >= timeexpire;
//}
//
//void MailTemplate::ToKKSG(KKSG::SMailTemplate& mail)
//{
//	mail.set_uid(uid);
//	mail.set_timestamp(timestamp);
//	mail.set_timeexpire(timeexpire);
//	mail.set_title(extra.tile);
//	mail.set_content(extra.content);
//	for (auto i = extra.items.begin(); i != extra.items.end(); ++i)
//	{
//		KKSG::ItemBrief* item = mail.add_items();
//		item->set_itemid(i->ItemID);
//		item->set_itemcount(i->ItemCount);
//	}
//}
//
//void MailTemplate::ToThis(const KKSG::SMailTemplate& mail)
//{
//	uid = mail.uid();
//	timestamp = mail.timestamp();
//	timeexpire = mail.timeexpire();
//	extra.tile = mail.title();
//	extra.content = mail.content();
//	for (int i = 0; i < mail.items_size(); ++i)
//	{
//		extra.items.push_back(ItemDesc(mail.items(i).itemid(), mail.items(i).itemcount()));
//	}
//}
////////////////////////////////////////////////////////////////////////////////////////////////
//bool MailMgr::Init()
//{
//	m_isloaded = false;
//	return true;
//}
//
//void MailMgr::Uninit()
//{
//	for (auto i = m_systemmails.begin(); i != m_systemmails.end(); ++i)
//	{
//		if (NULL != i->second)
//		{
//			delete i->second;
//		}
//	}
//	for (auto j = m_roleid2mails.begin(); j != m_roleid2mails.end(); ++j)
//	{
//		if (NULL != j->second)
//		{
//			delete j->second;
//		}
//	}
//}
//
//MailAll* MailMgr::GetRoleMails(UINT64 roleid)
//{
//	auto i = m_roleid2mails.find(roleid);
//	if (i == m_roleid2mails.end())
//	{
//		MailAll* mails = new MailAll();
//		m_roleid2mails[roleid] = mails;
//		return mails;
//	}
//	return i->second;
//}
//
//MailTemplate* MailMgr::GetSystemMail(UINT64 uid)
//{
//	auto i = m_systemmails.find(uid);
//	if (i == m_systemmails.end())
//	{
//		return NULL;
//	}
//	return i->second;
//}
//
//bool MailMgr::AddSystemMail(UINT32 expiretime, const std::string& title, const std::string& content, std::vector<ItemDesc>* items)
//{
//	if (items->size() > MAIL_ITEM)
//	{
//		return false;
//	}
//	MailTemplate* mail = new MailTemplate();
//	mail->uid = UIDHelper::CreateUID(UID_Mail);
//	mail->timestamp = (UINT32)time(NULL);
//	mail->timeexpire = mail->timestamp + ((0 == expiretime) ? MAIL_EXPIRE_TIME : expiretime);
//	mail->extra.tile = title;
//	mail->extra.content = content;
//	if (NULL != items && !items->empty())
//	{
//		mail->extra.items = *items;
//	}
//	m_systemmails[mail->uid] = mail;
//
//	BroadCastSystemMail(mail->uid, mail->timeexpire, mail->timestamp, mail->IsAttach());
//
//	SaveSystemMail(mail);
//
//	return true;
//}
//
//void MailMgr::BroadCastSystemMail(UINT64 uid, UINT32 timeexpire, UINT32 timestamp, bool hasattach)
//{
//	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
//	{
//		MailSystemOp op(*i);
//		op.Add(uid, timeexpire, timestamp, hasattach);
//	}
//}
//
//void MailMgr::QuerySystemMail()
//{
//	if (!m_isloaded)
//	{
//		PtcG2D_QueryMailTemplateReport report;
//		DBLink::Instance()->Send(report);
//	}
//}
//
//void MailMgr::LoadSystemMail(const KKSG::SMailTemplate* mail)
//{
//	m_isloaded = true;
//	MailTemplate* mailtemplate = new MailTemplate;
//	mailtemplate->ToThis(*mail);
//	if (!mailtemplate->IsExpire())
//	{
//		m_systemmails[mailtemplate->uid] = mailtemplate;
//	}
//}
//
//void MailMgr::SaveSystemMail(MailTemplate* mail)
//{
//	PtcG2D_AddMailTemplateReport report;
//	KKSG::SMailTemplate* smail = report.m_Data.mutable_mail();
//	mail->ToKKSG(*smail);
//	DBLink::Instance()->Send(report);
//}
//
//UINT32 MailMgr::Fetch(Role* role, UINT32& page, UINT32 count, std::vector<Mail*>& tempmails)
//{
//	std::map<UINT64, Mail*> mails;
//
//	MailRecord* record = role->Get<MailRecord>();
//	record->Update();
//	AddMail(record->GetSystemMails(), mails);
//	MailRoleOp op(role->GetID());
//	op.Update();
//	AddMail(op.GetMails(), mails);
//
//	UINT32 allcount = mails.size();
//	UINT32 allpage = (UINT32)(allcount/count);
//	UINT32 allleft = allcount%count;
//	if (0 != allpage && 0 == allleft)
//	{
//		allpage -= 1;
//	}
//	// invalid page
//	if (page > allpage)
//	{
//		page = allpage;	
//	}
//	UINT32 begin = page*count;
//	UINT32 k = 0; 
//	auto i = mails.rbegin();
//	for (std::advance(i, begin); i != mails.rend(); ++i)
//	{
//		if (k >= count)
//		{
//			break;
//		}
//		tempmails.push_back(i->second);
//		++k;
//	}
//	return allpage;
//}
//
//void MailMgr::AddMail(MailAll* mailall, std::map<UINT64, Mail*>& mails)
//{
//	if (NULL != mailall)
//	{
//		for (auto i = mailall->mails.begin(); i != mailall->mails.end(); ++i)
//		{
//			if (!i->second->IsDelete())
//			{
//				mails[i->first] = i->second;
//			}
//		}
//	}
//}
//
//void MailMgr::HintCheck(Role* role)
//{
//	bool ishint = false;
//
//	MailSystemOp systemop(role);
//	systemop.Update();
//	ishint |= systemop.IsHint();
//
//	MailRoleOp roleop(role->GetID());
//	roleop.Update();
//	ishint |= roleop.IsHint();
//
//	role->HintNotify(MailId, !ishint);
//}
