#ifndef _H_Account_Session_H__
#define _H_Account_Session_H__

#include "pb/project.pb.h"
#include "logoutlistener.h"
#include "role/role.h"


enum EAccountState
{
	ACCOUNT_STATE_INVALID,
	ACCOUNT_STATE_IN_LOBBY,
	ACCOUNT_STATE_SELECTING_ROLE,
	ACCOUNT_STATE_CREATE_ROLE,
	ACCOUNT_STATE_IN_GAME,
	ACCOUNT_STATE_LOGGING_OUT,
};

// 共享内存里的AccountSession，POD类型
struct CShmAccountSession
{
	SessionType		m_sessionID;
	UINT64			m_qwRoleID;
	UINT64			m_roleIdIndex[ACCOUNT_MAX_ROLE_NUM];
	EAccountState	m_nState;
	char			m_accountID[MAX_ACCOUNT_LEN];		//tencent openid
	UINT32			m_clientInfoLen;
	char			m_clientInfo[MAX_CLIENT_INFO_BYTE];
	UINT32			m_dwLoginServerId;
	UINT32			m_dwServerId;
	bool			m_accountBackFlow;
};

class CAccountSession
{
public:
	CAccountSession();
	~CAccountSession();
	friend class CAccountSessionMgr;

	bool Init(SessionType sessionID);
	bool Init(CShmAccountSession* shmData);

	bool IsProfessionValid(INT32 nSlot);
	bool IsProfessionSlotEmpty(INT32 nSlot);
	INT32 GetProfessionCount();

public:
	void SetSessionID(SessionType sessionID)			{ m_poShmData->m_sessionID = sessionID; }
	void SetAccountID(const std::string& strAccountID);
	void SetState(EAccountState nState)					{ m_poShmData->m_nState = nState; }
	void SetRole(CRole* poRole);
	void SetLoginToken(const std::string& strToken)		{ m_loginToken = strToken; }

	SessionType GetSessionID() const					{ return m_poShmData->m_sessionID; }
	const std::string& GetAccountID() const				{ return m_accountID; }
	CRole* GetRole()									{ return m_poRole; }
	UINT64 GetRoleID() const							{ return m_poShmData->m_qwRoleID; }
	EAccountState GetState() const						{ return m_poShmData->m_nState; }
	const std::string& GetLoginToken() const			{ return m_loginToken; }

	void SetClientConf(const KKSG::ClientInfo& client);
	KKSG::ClientInfo& GetCliConf()						{ return m_cliConf; }
	const KKSG::ClientInfo& GetCliConf() const			{ return m_cliConf; }
	KKSG::PlatType GetPlat() const						{ return (KKSG::PlatType)m_cliConf.platid(); }
	void UpdateClientConfToShm();

	UINT64 GetRoleIdBySlot(INT32 nSlot);
	void LoadAllRoleData(const KKSG::LoadAccountData& data);
	void LoadRoleData(INT32 nSlot, const std::string& data);
	void SetRoleId(INT32 nSlot, UINT64 roleId);

	UINT32 GetLoginServerId() { return m_poShmData->m_dwLoginServerId; }
	void SetLoginServerId(UINT32 serverId) { m_poShmData->m_dwLoginServerId = serverId; }
	UINT32 GetServerId() { return m_poShmData->m_dwServerId; }
	void SetServerId(UINT32 serverId) { m_poShmData->m_dwServerId = serverId; }
	bool IsAccountBackFlow() { return m_poShmData->m_accountBackFlow; }
	void SetAccountBackFlow(bool back) { m_poShmData->m_accountBackFlow = back; }

	bool CheckSelectRole();
	void OnSelectRoleBegin();
	void OnSelectRoleEnd();
private:
	CShmAccountSession* m_poShmData;

	std::string m_accountID;		// tencent openid
	std::string m_loginToken;		// 不放共享内存，丢失影响不大

	CRole*			m_poRole;		//Current Role

	//bool   m_isgm;
	KKSG::ClientInfo		m_cliConf;

	UINT64			m_qwBeginSelRoleTime;
};

#endif