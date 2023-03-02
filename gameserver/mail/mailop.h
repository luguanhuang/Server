//#ifndef __MAILOP_H__
//#define __MAILOP_H__
//
//class Role;
//class Mail;
//struct MailAll;
//struct ItemDesc;
//class IMailOp
//{
//public:
//	IMailOp();
//	virtual ~IMailOp();
//
//	bool IsHint();
//	Mail* GetMail(UINT64 mailid);
//	MailAll* GetMails() { return m_mails; }
//
//	virtual void OnLogin(Role* role) { };
//	virtual void OnLogOut(Role* role) { };
//
//	virtual void Update();
//	virtual bool Read(Mail* mail);
//	virtual bool Reward(Role* role, Mail* mail);
//	virtual bool Delete(Mail* mail);
//
//
//protected:
//	MailAll* m_mails;
//};
//
//class MailRoleOp : public IMailOp
//{
//public:
//	MailRoleOp(UINT64 roleid);
//	virtual ~MailRoleOp();
//
//	virtual void OnLogin(Role* role);
//
//	virtual void Update();
//	virtual bool Read(Mail* mail);
//	virtual bool Reward(Role* role, Mail* mail);
//	virtual bool Delete(Mail* mail);
//
//	bool Add(UINT64 srcid, UINT32 type, const std::string& title, const std::string& content, std::vector<ItemDesc>* items = NULL, UINT32 expiretime = 0);
//	void UpdateDb(UINT32 op, Mail* mail);
//
//private:
//	UINT64 m_roleid;
//};
//
//class MailSystemOp : public IMailOp
//{
//public:
//	MailSystemOp(Role* role);
//	virtual ~MailSystemOp();
//
//	virtual void OnLogin(Role* role);
//
//	virtual bool Read(Mail* mail);
//	virtual bool Reward(Role* role, Mail* mail);
//	virtual bool Delete(Mail* mail);
//
//	bool Add(UINT64 uid, UINT32 timeexpire, UINT32 timestamp, bool hasattach, bool isnotice = true);
//
//private:
//	Role* m_role;
//};
//
//#endif