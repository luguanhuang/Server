#ifndef __MAILMGR_H__
#define __MAILMGR_H__

#include "maildata.h"
#include "pb/project.pb.h"
#include "timer.h"
#include "lructrl.h"
#include "mailfunctor.h"

class CRole;
class PtcM2D_MailOperation;
class CMailMgr
{
	CMailMgr();
	~CMailMgr();
	DECLARE_SINGLETON(CMailMgr);
public:
	bool Init();
	void Uninit();
	void Update();

	void OnRoleLogin(CRole* role);
	void OnReadRoleMailFromDB(CRole* role, const KKSG::ReadMailFromDBRes &roRes);


	//发邮件
	bool SendMail(UINT64 qwRoleID, const MailData& roData);
	bool SendMail(UINT64 qwRoleID, UINT32 templateId);
	bool SendMail(UINT64 qwRoleID, UINT32 templateId, const std::vector<ItemDesc>& items);
	bool SendMail(UINT64 qwRoleID, const KKSG::SMail& oSaveData, bool isGlobal, UINT64 globalId, UINT32 lastsend);

	bool SendSystemMail(const MailData& roData);
	bool SendSystemMail(UINT32 templateId);
	bool SendGuildMail(UINT64 guildId, const MailData& roData);
	bool SendGuildMail(UINT64 guildId, UINT32 templateId);


	//个人操作邮件
	//dwPage:			输入(请求页号) 输出(真实页号) (0开始)
	//dwPageNum:		输出(总页数)
	//dwCountPerPage:	输入(每页条数)
	bool Fetch(UINT64 qwRoleID, UINT32& dwPage, UINT32& dwPageNum, UINT32 dwCountPerPage, std::vector<KKSG::SMail*>& roMailList);
	bool Read(UINT64 qwRoleID, UINT64 qwMailID);
	
	bool Reward(UINT64 qwRoleID, UINT64 qwMailID, CRole* role, UINT32 delayid);
	bool RewardAll(UINT64 qwRoleID, CRole* role, UINT32 delayid);
	void DoReward(UINT64 qwRoleID, UINT64 qwMailID, bool isSucceed);	

	bool Delete(UINT64 qwRoleID, UINT64 qwMailID);

	bool DeleteAllMail(UINT64 qwRoleID, bool bByIdip = false);//idip清空角色时需要调用;
	//hint
	void SendHintNotify(CRole* role, bool istrue);

	void DoTxLog(UINT64 qwRoleID, const KKSG::SMail& data, INT32 type);
	void Test();


	//Role Mail Data operation
	void AddRoleMailInfo(UINT64 qwRoleID);
	void DelRoleMaillInfo(UINT64 qwRoleID);
	std::map<UINT64, KKSG::SMail>* GetRoleMailInfo(UINT64 qwRoleID);

private:
	bool _ReadAllRoleMailFromDB();
	bool _ReadAllGlobalMailFromDB();
	bool _ReadAllRoleRecordFromDB();

	void _AddTemplateMailToDB(const KKSG::SMail& data);
	void _UpdateToDB(UINT64 qwRoleID, KKSG::MailOpType op, const KKSG::SMail& data, bool bByTemplate = false, UINT64 qwTemplateID = 0);

	bool _AddRoleMail(UINT64 qwRoleID, const KKSG::SMail& data);
	bool _AddGlobalMail(const KKSG::SMail& data);

	void _UpdateRoleMaxGlobalMailID(UINT64 qwRoleID, UINT64 qwMaxGlobalMailID);
	UINT64 _GetRoleMaxGlobalMailID(UINT64 qwRoleID);

	void _MailFull(CRole* role);
	bool _InnerDelete(std::map<UINT64, KKSG::SMail>& mails, UINT64 mailid, UINT64 roleid);

	void _ProcessRoleMailLogin(CRole* role);

private:
	struct DummyMail
	{
		DummyMail()
			:roleId(0), isGlobal(false), globalId(0), latesend(0) {}

		UINT64 roleId;
		KKSG::SMail mailData;
		bool isGlobal;
		UINT64 globalId;
		UINT32 latesend;
	};

	void DoSendMail(DummyMail* mail);
    bool OnAutoCleanMail();

private:
	typedef std::map<UINT64, KKSG::SMail>	CMailMap;			//mailID -> mailData
	typedef std::map<UINT64, CMailMap>		CAllRoleMailMap;	//roleID -> mailMap
	typedef std::map<UINT64, UINT64>		CMaxMailIDMap;		//roleID -> mailID
	typedef CLruControl<UINT64, CMailDataAddFunctor, CMailDataDelFunctor>	CRoleMailCacheCtrl;

	CMailMap* GetMails(UINT64 roleid);

	CMailMap		m_oGlobalMailInfo;
	CAllRoleMailMap	m_oRoleMailInfo;
	CMaxMailIDMap	m_oRoleMaxIDInfo;
	const static UINT32 kMaxSendPerFrame = 100;
	std::list<DummyMail*> m_lMail;				// 发送队列

	UINT32			m_dwCleanHandle;

	CRoleMailCacheCtrl	m_oMailCacheCtrl;

private:
	class ExpiredMailCleanTimer : public ITimer
	{
	public:
		ExpiredMailCleanTimer() :m_hTimerHandle(INVALID_HTIMER), m_dwTotalCount(0) {}
		virtual ~ExpiredMailCleanTimer() {}
		virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	public:
		HTIMER	m_hTimerHandle;
		UINT32	m_dwTotalCount;
		CAllRoleMailMap::iterator	m_itBeg;
		CAllRoleMailMap::iterator	m_itEnd;
	};

	ExpiredMailCleanTimer*	m_poCleanTimer;

	void SetMailCleanTimer(ExpiredMailCleanTimer* poTimer)	{ m_poCleanTimer = poTimer; }
	ExpiredMailCleanTimer* GetMailCleanTimer()				{ return m_poCleanTimer; }
};

#endif