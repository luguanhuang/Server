#ifndef _H_MySql_Mgr_H__
#define _H_MySql_Mgr_H__

#include "mutex.h"
#include "mysqltask.h"
#include "mysqlthread.h"


class CMysqlMgr
{
	CMysqlMgr();
	~CMysqlMgr();
	DECLARE_SINGLETON(CMysqlMgr)

public:
	bool Init();
	void Uninit();


	void Process();

	void PushTask(CMysqlTask* poTask, UINT32 dwThrdIndex = 0); 
	CMysqlThread* GetThread(UINT32 dwIndex);

	void PushOnlineNumTask(CMysqlTask* poTask); 
	UINT32 GetThreadNum() const { return m_oThreadList.size(); }

	void DeleteEmptyAccount(IMysqlConn* poMysqlConn);
	bool ReadCurrentRegisterNum(IMysqlConn* poMysqlConn);
	void UpdateAccountServerId(IMysqlConn* poMysqlConn);
	void UpdateRankListServerId(IMysqlConn* poMysqlConn);

	void UpdateMysqlStatus();
	static void MysqlEventCallBack(EMysqlConnEvent nEvent);

	void SetMSSaveDBEnd(bool isEnd);
	bool GetMSSaveDBEnd() { return m_bMSSaveDBEnd; }

private:
	typedef std::vector<CMysqlThread*> CMysqlThreadList;
	CMysqlThreadList	m_oThreadList;
	CMysqlThread		m_oOnlineThread;

	bool				m_bMysqlOK;
	bool				m_bKickNtfSended;
	Mutex				m_oMysqlEvtMux;
	std::vector<INT32>	m_oMysqlEvtList;

	bool				m_bMSSaveDBEnd;
};


UINT32 GetAccountThreadIndex();
UINT32 GetRoleThreadIndex(UINT64 qwRoleID);

#endif