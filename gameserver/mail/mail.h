//#ifndef __MAIL_H__
//#define __MAIL_H__
//
//#include "unit/roleserilizeinterface.h"
//
//namespace KKSG
//{
//	class SMail;
//	class SMailAll;
//};
//struct ItemDesc;
//
//struct MailExtra
//{
//	std::string tile;
//	std::string content;
//	std::vector<ItemDesc> items;
//};
//
//class Mail
//{
//public:
//	Mail();
//	virtual ~Mail();
//
//	virtual void ToThis(const KKSG::SMail& mail);
//	virtual void ToKKSG(KKSG::SMail& mail);
//
//	virtual MailExtra* GetExtra() { return NULL; } 
//
//	int GetExpire();
//	inline void SetExpire() { isdelete = true; isread = true; }
//	inline bool IsDelete() { return isdelete; }
//
//public:
//	UINT64 uid;
//	UINT32 type;
//	bool isread;
//	bool isdelete;
//	bool istemplate;
//	UINT32 attachment;
//	UINT32 timestamp;
//	UINT32 timeexpire;
//};
//
//class SystemMail : public Mail
//{
//public:
//	SystemMail();
//	virtual ~SystemMail();
//
//	virtual void ToKKSG(KKSG::SMail& mail);
//
//	virtual MailExtra* GetExtra(); 
//};
//
//class RoleMail : public Mail
//{
//public:
//	RoleMail();
//	virtual ~RoleMail();
//
//	virtual void ToThis(const KKSG::SMail& mail);
//	virtual void ToKKSG(KKSG::SMail& mail);
//
//	virtual MailExtra* GetExtra() {return &extra; }
//
//public:
//	UINT64 srcid;
//	std::string srcname;
//
//	MailExtra extra;
//};
//
//struct MailAll
//{
//	Switch m_ismodify;
//	std::map<UINT64, Mail*> mails; 
//
//	MailAll();
//	~MailAll();
//
//	bool IsHint();
//	bool Update();
//};
//
//#endif