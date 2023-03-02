#include "pch.h"
//#include "dblink.h"
//#include "mailop.h"
//#include "maildef.h"
//#include "mailmgr.h"
//#include "mailrecord.h"
//#include "unit/role.h"
//#include "gamelogic/item.h"
//#include "unit/role.h"
//#include "unit/rolemanager.h"
//#include "gamelogic/systemiddef.h"
//#include "gamelogic/rolesummarymgr.h"
//#include "gamelogic/bagtransition.h"
//#include "mail/ptcg2d_querymailreport.h"
//#include "mail/ptcg2d_mailopreport.h"
//
//enum
//{
//	DB_Add = 1,
//	DB_Update = 2,
//	DB_Delete = 3,
//};
//
//IMailOp::IMailOp()
//{
//	m_mails = NULL;
//}
//
//IMailOp::~IMailOp()
//{
//
//}
//
//void IMailOp::Update()
//{
//	m_mails->Update();
//}
//
//bool IMailOp::IsHint()
//{
//	return m_mails->IsHint();
//}
//
//Mail* IMailOp::GetMail(UINT64 mailid)
//{
//	auto i = m_mails->mails.find(mailid);
//	if (i == m_mails->mails.end())
//	{
//		return NULL;
//	}
//	return i->second;
//}
//
//bool IMailOp::Read(Mail* mail)
//{
//	mail->isread = true;
//	return true;
//}
//
//bool IMailOp::Reward(Role* role, Mail* mail)
//{
//	if (Mail_Given != mail->attachment)
//	{
//		return false;
//	}
//	MailExtra* extra = mail->GetExtra();
//	if (NULL != extra)
//	{
//		BagGiveItemTransition trans(role);
//		trans.SetReason(ItemFlow_Mail, ItemFlow_Mail_Give);
//		for (auto j = extra->items.begin(); j != extra->items.end(); ++j)
//		{
//			trans.GiveItem(j->ItemID, j->ItemCount);
//		}
//		trans.NotifyClient();
//	}
//	mail->attachment = Mail_Taken;
//	return true;
//}
//
//bool IMailOp::Delete(Mail* mail)
//{
//	mail->isdelete = true;
//	return true;
//}
////////////////////////////////////////////////////////////////////////////////////////
//MailRoleOp::MailRoleOp(UINT64 roleid)
//{
//	m_roleid = roleid;
//	m_mails = MailMgr::Instance()->GetRoleMails(m_roleid);
//}
//
//MailRoleOp::~MailRoleOp()
//{
//
//}
//
//void MailRoleOp::Update()
//{
//	IMailOp::Update();
//	for (auto i = m_mails->mails.begin(); i != m_mails->mails.end();)
//	{
//		if (i->second->IsDelete())
//		{
//			UpdateDb(DB_Delete, i->second);
//			m_mails->mails.erase(i++);
//		}
//		else
//		{
//			++i;
//		}
//	}
//}
//
//void MailRoleOp::OnLogin(Role* role)
//{
//	PtcG2D_QueryMailReport ptc;
//	ptc.m_Data.set_roleid(role->GetID());
//	DBLink::Instance()->Send(ptc);
//}
//
//bool MailRoleOp::Add(UINT64 srcid, UINT32 type, const std::string& title, const std::string& content
//	, std::vector<ItemDesc>* items, UINT32 expiretime)
//{
//	if (NULL != items && items->size() > MAIL_ITEM)
//	{
//		return false;
//	}
//	RoleMail* mail = new RoleMail();
//	if (0 != expiretime)
//	{
//		mail->timeexpire = mail->timestamp + expiretime;
//	}
//	mail->type = type;
//	if (type == Mail_Role)
//	{
//		std::string name = RoleSummaryMgr::Instance()->FindRoleName(srcid);
//		mail->srcid = srcid;
//		mail->srcname = name;
//	}
//	mail->extra.tile = title;
//	mail->extra.content = content;
//	if (NULL != items && !items->empty())
//	{
//		mail->attachment = Mail_Given;
//		mail->extra.items = *items;
//	}
//	m_mails->mails[mail->uid] = mail;
//
//	UpdateDb(DB_Add, mail);
//
//	Role* role = RoleManager::Instance()->FindByRoleID(m_roleid);
//	if (NULL != role)
//	{
//		role->HintNotify(MailId, false);
//	}
//	return true;
//}
//
//bool MailRoleOp::Read(Mail* mail)
//{
//	IMailOp::Read(mail);
//
//	UpdateDb(DB_Update, mail);
//
//	return true;
//}
//
//bool MailRoleOp::Reward(Role* role, Mail* mail)
//{
//	IMailOp::Reward(role, mail);
//
//	UpdateDb(DB_Update, mail);
//
//	return true;
//}
//
//bool MailRoleOp::Delete(Mail* mail)
//{
//	IMailOp::Delete(mail);
//
//	auto i = m_mails->mails.find(mail->uid);
//	if (i == m_mails->mails.end())
//	{
//		return false;
//	}
//	m_mails->mails.erase(i);
//
//	UpdateDb(DB_Delete, mail);
//
//	return true;
//}
//
//void MailRoleOp::UpdateDb(UINT32 op, Mail* mail)
//{
//	PtcG2D_MailOpReport ptc;
//	ptc.m_Data.set_roleid(m_roleid);
//	ptc.m_Data.set_mailuid(mail->uid);
//	ptc.m_Data.set_op(op);
//	if (DB_Add == op || DB_Update == op)
//	{
//		KKSG::SMail* smail = ptc.m_Data.mutable_mail();
//		mail->ToKKSG(*smail);
//	}
//	DBLink::Instance()->Send(ptc);
//}
//
///////////////////////////////////////////////////////////////////////////////////////////
//MailSystemOp::MailSystemOp(Role* role)
//{
//	m_role = role;
//	m_mails = role->Get<MailRecord>()->GetSystemMails();
//}
//
//MailSystemOp::~MailSystemOp()
//{
//
//}
//
//bool MailSystemOp::Add(UINT64 uid, UINT32 timeexpire, UINT32 timestamp, bool hasattach, bool isnotice)
//{
//	if (m_role->GetCreateTime() > timestamp)
//	{
//		return false;
//	}
//	SystemMail* mail = new SystemMail();
//	mail->timestamp = timestamp;
//	mail->timeexpire = timeexpire;
//	mail->istemplate = true;
//	mail->type = Mail_System;
//	mail->attachment = (hasattach) ? Mail_Given : Mail_None;
//	mail->uid = uid;
//	m_mails->mails[mail->uid] = mail;
//
//	m_mails->m_ismodify.Set();
//
//	if (isnotice)
//	{
//		m_role->HintNotify(MailId, false);
//	}
//
//	return true;
//}
//
//bool MailSystemOp::Read(Mail* mail)
//{
//	IMailOp::Read(mail);
//	m_mails->m_ismodify.Set();
//	return true;
//}
//
//bool MailSystemOp::Reward(Role* role, Mail* mail)
//{
//	IMailOp::Reward(role, mail);
//	m_mails->m_ismodify.Set();
//	return true;
//}
//
//bool MailSystemOp::Delete(Mail* mail)
//{
//	IMailOp::Delete(mail);
//	m_mails->m_ismodify.Set();
//	return true;
//}
//
//void MailSystemOp::OnLogin(Role* role)
//{
//	// check new system mails
//	std::map<UINT64, MailTemplate*>* systemmails = MailMgr::Instance()->GetSystemMails();
//	for (auto i = systemmails->begin(); i != systemmails->end(); ++i)
//	{
//		if (!i->second->IsExpire())	
//		{
//			auto j = m_mails->mails.find(i->first);
//			if (j == m_mails->mails.end())
//			{
//				if (Add(i->first, i->second->timeexpire, i->second->timestamp, i->second->IsAttach(), false))
//				{
//					m_mails->m_ismodify.Set();
//				}
//			}
//		}
//	}
//	// check invalid system mails
//	//for (auto k = m_mails->mails.begin(); k != m_mails->mails.end(); ++k)
//	//{
//	//	if (!k->second->IsDelete())
//	//	{
//	//		auto l = systemmails->find(k->first);
//	//		if (l == systemmails->end())
//	//		{
//	//			k->second->SetExpire();
//	//		}
//	//	}
//	//}
//}
