#include "pch.h"
#include "role.h"
#include "pb/project.pb.h"
#include "network/gatelink.h"
#include "crpc.h"
#include "protocol.h"
#include "network/gslink.h"
#include "network/loginlink.h"
#include "network/rslink.h"
#include "shm/shmmgr.h"
#include "util/utilstring.h"
#include "idutil.h"
#include "scene/sceneconfig.h"


CRole::CRole()
:m_poShmData(NULL)
{
}

CRole::~CRole()
{
	if (m_poShmData)
	{
		ShmMgr::Instance()->GetRoleShm().Remove(m_poShmData->m_qwID);
		m_poShmData = NULL;
	}
}

bool CRole::Init(const KKSG::RoleAllInfo& roInfo, UINT64 sessionID)
{
	m_poShmData = ShmMgr::Instance()->GetRoleShm().Get(roInfo.brief().roleid());
	m_poShmData->m_sessionID = sessionID;
	m_poShmData->m_qwID    = roInfo.brief().roleid();
	m_poShmData->m_dwMapID	= roInfo.brief().sceneid();
	m_poShmData->m_dwServerId = GetServerIDByRoleID(m_poShmData->m_qwID);
	m_poShmData->m_dwStateAdjustBeginTime = 0;
	m_accountID = roInfo.brief().accountid();
	m_strName   = roInfo.brief().name();

	UtilString::SafeStringCopy(m_poShmData->m_accountID, m_accountID.c_str(), MAX_ACCOUNT_LEN);
	UtilString::SafeStringCopy(m_poShmData->m_strName, m_strName.c_str(), MAX_NAME_LEN);

	return true;
}

bool CRole::Init(CShmRole* roShmRole)
{
	if (roShmRole == NULL)
	{
		return false;
	}
	m_poShmData = roShmRole;

	m_poShmData->m_accountID[MAX_ACCOUNT_LEN - 1] = '\0';
	m_accountID = std::string(m_poShmData->m_accountID);
	m_poShmData->m_strName[MAX_NAME_LEN - 1] = '\0';
	m_strName   = std::string(m_poShmData->m_strName);

	// 离开场景中的，当成失败，继续保留原场景
	if (GetState() == ROLE_STATE_LEAVE_SCENE)
	{
		SetState(ROLE_STATE_IN_GAME);
		ClearSwitchInfo();
	}
	// 进入场景中的，当成成功，切到目标场景
	else if (GetState() == ROLE_STATE_ENTER_SCENE)
	{
		SetGsLine(GetDestGsLine());
		SetInCrossGs(GetDestIsCross());
		SetMapID(GetDestMapID());
		SetSceneID(GetDestSceneID()); // 这个可能不准，可能是动态生成的
		SetState(ROLE_STATE_IN_GAME);
		ClearSwitchInfo();
	}

	return true;
}

void CRole::Uninit()
{
}

void CRole::OnChangeScene()
{
	SetGsLine(INVALID_LINE_ID);
	SetState(ROLE_STATE_ENTER_SCENE);
	SetMapID(INVALID_MAP_ID);
	SetSceneID(INVALID_SCENE_ID);
	SetInCrossGs(false);
}

void CRole::OnLogin()
{
}

void CRole::OnLogout()
{
}

void CRole::Send(const CProtocol &roPtc)
{
	if (m_poShmData->m_sessionID == INVALID_SESSION_ID)
	{
		return;
	}
	roPtc.m_sessionID = m_poShmData->m_sessionID;
	GateLink::Instance()->SendToSession(roPtc);
}

void CRole::SendMsgToGS(const CProtocol& msg)
{
	msg.m_sessionID = m_poShmData->m_sessionID;

	// 本服GS直接发送
	// 跨服GS发给Router
	if (!m_poShmData->m_isCrossGs)
	{
		GSLink::Instance()->SendToLine(GetGsLine(), msg);
	}
	else
	{
		RSLink::Instance()->SendTo(msg);
	}
}

void CRole::SendMsgToGS(CRpc& msg)
{
	msg.m_sessionID = m_poShmData->m_sessionID;

	// 本服GS直接发送
	// 跨服GS发给Router
	if (!m_poShmData->m_isCrossGs)
	{
		GSLink::Instance()->SendToLine(GetGsLine(), msg);
	}
	else
	{
		RSLink::Instance()->SendTo(msg);
	}
}

bool CRole::CanLoginReconnect()
{
	if (!ControlConfig::Instance()->CanLoginReconnect())
	{
		return false;
	}
	if (IsWathcer())
	{
		return false;
	}
	if (!m_poShmData->m_dwMapID)
	{
		return false;
	}
	const SceneConf* pConf = CSceneConfig::Instance()->GetSceneConf(m_poShmData->m_dwMapID);
	if (pConf == NULL)
	{
		return false;
	}
	return pConf->CanReconnect;
}

