#include "pch.h"
//#include "time.h"
//#include "mail.h"
//#include "maildef.h"
//#include "mailmgr.h"
//#include "gamelogic/item.h"
//#include "utility/uidhelper.h"
//#include "pb/project.pb.h"
//
//Mail::Mail()
//{
//	uid = UIDHelper::CreateUID(UID_Mail);
//	isread = false;
//	isdelete = false;
//	istemplate = false;
//	type = Mail_System;
//	attachment = Mail_None;
//	timestamp = (UINT32)time(NULL);
//	timeexpire = timestamp + MAIL_EXPIRE_TIME;
//}
//
//Mail::~Mail()
//{
//
//}
//
//void Mail::ToThis(const KKSG::SMail& mail)
//{
//	uid = mail.uid();
//	type = mail.type();
//	isread = mail.isread();
//	isdelete = mail.isdelete();
//	istemplate = mail.istemplate();
//	attachment = mail.state();
//	timestamp = mail.timestamp();
//	timeexpire = mail.timeexpire();
//}
//
//void Mail::ToKKSG(KKSG::SMail& mail)
//{
//	mail.set_uid(uid);
//	mail.set_type(type);
//	mail.set_isread(isread);
//	mail.set_isdelete(isdelete);
//	mail.set_istemplate(istemplate);
//	mail.set_state(attachment);
//	mail.set_timestamp(timestamp);
//	mail.set_timeexpire(timeexpire);
//}
//
//int Mail::GetExpire()
//{
//	UINT32 nowtime = (UINT32)time(NULL);
//	if (nowtime >= timeexpire)
//	{
//		return 0;
//	}
//	return (int)(timeexpire - nowtime);
//}
/////////////////////////////////////////////////////////////////////////////////////////
//SystemMail::SystemMail()
//{
//
//}
//
//SystemMail::~SystemMail()
//{
//
//}
//
//void SystemMail::ToKKSG(KKSG::SMail& mail)
//{
//	Mail::ToKKSG(mail);
//	
//	MailExtra* extra = GetExtra();
//	if (NULL != extra)
//	{
//		mail.set_title(extra->tile);
//		mail.set_content(extra->content);
//		for (auto i = extra->items.begin(); i != extra->items.end(); ++i)
//		{
//			KKSG::ItemBrief* item = mail.add_items();
//			item->set_itemid(i->ItemID);
//			item->set_itemcount(i->ItemCount);
//		}
//	}
//}
//
//MailExtra* SystemMail::GetExtra()
//{
//	MailTemplate* mailtemplate = MailMgr::Instance()->GetSystemMail(uid);
//	if (NULL != mailtemplate)
//	{
//		return &mailtemplate->extra;
//	}
//	return NULL;
//}
//
/////////////////////////////////////////////////////////////////////////////////////////
//RoleMail::RoleMail()
//{
//
//}
//
//RoleMail::~RoleMail()
//{
//
//}
//
//void RoleMail::ToThis(const KKSG::SMail& mail)
//{
//	Mail::ToThis(mail);
//
//	srcid = mail.srcid();
//	srcname = mail.srcname();
//
//	extra.tile = mail.title();
//	extra.content = mail.content();
//
//	extra.items.clear();
//	for (int i = 0; i < mail.items_size(); ++i)
//	{
//		extra.items.push_back(ItemDesc(mail.items(i).itemid(), mail.items(i).itemcount()));
//	}
//}
//
//void RoleMail::ToKKSG(KKSG::SMail& mail)
//{
//	Mail::ToKKSG(mail);
//
//	mail.set_srcid(srcid);
//	mail.set_srcname(srcname);
//
//	mail.set_title(extra.tile);
//	mail.set_content(extra.content);
//
//	for (auto i = extra.items.begin(); i != extra.items.end(); ++i)
//	{
//		KKSG::ItemBrief* item = mail.add_items();
//		item->set_itemid(i->ItemID);
//		item->set_itemcount(i->ItemCount);
//	}
//}
/////////////////////////////////////////////////////////////////////////////////////////
//MailAll::MailAll()
//{
//
//}
//
//MailAll::~MailAll()
//{
//	for (auto i = mails.begin(); i != mails.end(); ++i)
//	{
//		delete i->second;
//	}
//}
//
//bool MailAll::IsHint()
//{
//	for (auto i = mails.begin(); i != mails.end(); ++i)
//	{
//		if (!i->second->isread)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//bool MailAll::Update()
//{
//	bool ishint = false;
//	for (auto i = mails.begin(); i != mails.end(); ++i)
//	{
//		if (!i->second->IsDelete() && 0 == i->second->GetExpire())
//		{
//			i->second->SetExpire();
//			m_ismodify.Set();
//			ishint = true;
//		}
//	}
//	return ishint;
//}