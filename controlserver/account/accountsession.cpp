#include "pch.h"
#include "accountsession.h"
#include "util/utilstring.h"
#include "shm/shmmgr.h"
#include "timeutil.h"


CAccountSession::CAccountSession()
:m_poShmData(NULL)
,m_poRole(NULL)
,m_qwBeginSelRoleTime(0)
{
}

CAccountSession::~CAccountSession()
{
	if (m_poShmData)
	{
		ShmMgr::Instance()->GetAccountShm().Remove(m_poShmData->m_sessionID);
		m_poShmData = NULL;
	}
}

bool CAccountSession::Init(SessionType sessionID)
{
	m_poShmData = ShmMgr::Instance()->GetAccountShm().Get(sessionID);
	if (m_poShmData == NULL)
	{
		LogError("can't get shm");
		return false;
	}
	SetSessionID(sessionID);
	return true;
}

bool CAccountSession::Init(CShmAccountSession* shmData)
{
	if (shmData == NULL)
	{
		return false;
	}
	m_poShmData = shmData;

	m_poShmData->m_accountID[MAX_ACCOUNT_LEN - 1] = '\0';
	m_accountID = std::string(m_poShmData->m_accountID);

	return true;
}

void CAccountSession::SetRole(CRole* poRole)
{ 
	m_poRole = poRole; 
	if (m_poRole)
	{
		m_poShmData->m_qwRoleID = m_poRole->GetID();
	}
	else
	{
		m_poShmData->m_qwRoleID = 0;
	}
}

void CAccountSession::SetAccountID(const std::string& strAccountID)
{ 
	m_accountID = strAccountID; 
	UtilString::SafeStringCopy(m_poShmData->m_accountID, strAccountID.c_str(), MAX_ACCOUNT_LEN);
}

void CAccountSession::SetClientConf(const KKSG::ClientInfo& client)
{
	m_cliConf.CopyFrom(client);
	UpdateClientConfToShm();
}

void CAccountSession::UpdateClientConfToShm()
{
	UINT32 size = m_cliConf.ByteSize();
	LogDebug("client binary size: %u", size);
	if (size > MAX_CLIENT_INFO_BYTE)
	{
		LogError("client info binary size: %u is bigger than max %u", size, MAX_CLIENT_INFO_BYTE);
		return;
	}
	m_poShmData->m_clientInfoLen = size;
	m_cliConf.SerializeToArray((void*)m_poShmData->m_clientInfo, MAX_CLIENT_INFO_BYTE);
}

bool CAccountSession::IsProfessionValid(INT32 nSlot)
{
	if (nSlot < 0 || nSlot >= ACCOUNT_MAX_ROLE_NUM)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CAccountSession::IsProfessionSlotEmpty(INT32 nSlot)
{
	if (nSlot < 0 || nSlot >= ACCOUNT_MAX_ROLE_NUM)
	{
		return false;
	}
	return m_poShmData->m_roleIdIndex[nSlot] == 0;
}

INT32 CAccountSession::GetProfessionCount()
{
	INT32 nCount = 0;
	for (INT32 i = 0; i < ACCOUNT_MAX_ROLE_NUM; ++i)
	{
		if (m_poShmData->m_roleIdIndex[i])
		{
			nCount++;
		}
	}
	return nCount;
}

UINT64 CAccountSession::GetRoleIdBySlot(INT32 nSlot)
{
	if (nSlot < 0 || nSlot >= ACCOUNT_MAX_ROLE_NUM)
	{
		return 0;
	}
	return m_poShmData->m_roleIdIndex[nSlot];
}

void CAccountSession::SetRoleId(INT32 nSlot, UINT64 roleId)
{
	if (nSlot < 0 || nSlot >= ACCOUNT_MAX_ROLE_NUM)
	{
		return ;
	}
	m_poShmData->m_roleIdIndex[nSlot] = roleId;
}

void CAccountSession::LoadRoleData(INT32 nSlot, const std::string& data)
{
	if (nSlot < 0 || nSlot >= ACCOUNT_MAX_ROLE_NUM)
	{
		return ;
	}
	if (data.empty())
	{
		return;
	}
	KKSG::RoleBriefInfo  brief;
	if (!brief.ParseFromString(data))
	{
		LogError("parser role brief failed, account[%s], slot[%d]", GetAccountID().c_str(), nSlot);
		return;
	}
	SetRoleId(nSlot, brief.roleid());
}

void CAccountSession::LoadAllRoleData(const KKSG::LoadAccountData& data)
{
	memset(m_poShmData->m_roleIdIndex, 0, sizeof(m_poShmData));
	LoadRoleData(0, data.role1());
	LoadRoleData(1, data.role2());
	LoadRoleData(2, data.role3());
	LoadRoleData(3, data.role4());
	LoadRoleData(4, data.role5());
	LoadRoleData(5, data.role6());
	LoadRoleData(6, data.role7());
	LoadRoleData(7, data.role8());
	LoadRoleData(8, data.role9());
}

bool CAccountSession::CheckSelectRole()
{
	if(GetState() != ACCOUNT_STATE_SELECTING_ROLE)
	{
		return true;
	}
	else
	{
		UINT64 qwNowTime = TimeUtil::GetMilliSecond();
		if(qwNowTime > m_qwBeginSelRoleTime && qwNowTime - m_qwBeginSelRoleTime >= 30 * 1000)
		{
			SetState(ACCOUNT_STATE_IN_LOBBY);
			return true;
		}

		return false;
	}
}

void CAccountSession::OnSelectRoleBegin()
{
	SetState(ACCOUNT_STATE_SELECTING_ROLE);
	m_qwBeginSelRoleTime = TimeUtil::GetMilliSecond();
}

void CAccountSession::OnSelectRoleEnd()
{
	m_qwBeginSelRoleTime = 0;
	SetState(ACCOUNT_STATE_IN_LOBBY);
}
