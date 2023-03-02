//#ifndef __MAILMGR_H__
//#define __MAILMGR_H__
//
//#include "mail.h"
//
//namespace KKSG
//{
//	class SMailTemplate;
//}
//struct MailTemplate
//{
//	UINT64 uid;
//	UINT32 timestamp;
//	UINT32 timeexpire;
//	MailExtra extra;
//
//	bool IsExpire();
//	bool IsAttach() { return !extra.items.empty(); }
//
//	void ToKKSG(KKSG::SMailTemplate& mail);
//	void ToThis(const KKSG::SMailTemplate& mail);
//};
//
//class Role;
//class MailMgr
//{
//	DECLARE_SINGLETON(MailMgr);
//public:
//	bool Init();
//	void Uninit();
//
//	UINT32 Fetch(Role* role, UINT32& page, UINT32 count, std::vector<Mail*>& tempmails);
//
//	// system mail 
//	void QuerySystemMail();
//	void LoadSystemMail(const KKSG::SMailTemplate* mail);
//	void SaveSystemMail(MailTemplate* mail);
//
//	MailTemplate* GetSystemMail(UINT64 uid);
//	std::map<UINT64, MailTemplate*>* GetSystemMails() { return &m_systemmails; };
//
//	bool AddSystemMail(UINT32 expiretime, const std::string& title, const std::string& content, std::vector<ItemDesc>* items = NULL);
//
//	// role mail
//	MailAll* GetRoleMails(UINT64 roleid);
//
//	// common
//	void HintCheck(Role* role);
//
//private:
//	void BroadCastSystemMail(UINT64 uid, UINT32 timeexpire, UINT32 timestamp, bool hasattach);
//	void AddMail(MailAll* mailall, std::map<UINT64, Mail*>& mails);
//
//private:
//	bool m_isloaded;
//	HTIMER m_timer;
//	std::map<UINT64, MailTemplate*> m_systemmails;
//	std::unordered_map<UINT64, MailAll*> m_roleid2mails;
//};
//
//#endif