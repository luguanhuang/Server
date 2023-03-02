#include "pch.h"
#include "leavescenecontrol.h"
#include "unit/rolemanager.h"
#include "timeutil.h"
#include "crpc.h"


INSTANCE_SINGLETON(CLeaveSceneControl)


CLeaveSceneControl::CLeaveSceneControl()
:m_dwSerialID(0)
{
}

CLeaveSceneControl::~CLeaveSceneControl()
{
}

bool CLeaveSceneControl::Init()
{
	return true;
}

void CLeaveSceneControl::Uninit()
{
}

UINT32 CLeaveSceneControl::BeginLeave(UINT32 dwRpcDealyed, const std::vector<Role*>& roRoleList)
{
	if(roRoleList.empty()) return 0;

	CLeaveSceneReq oReq;
	for(std::vector<Role*>::size_type i = 0; i < roRoleList.size(); ++i)
	{
		Role* poRole = roRoleList[i];
		if(poRole == NULL)
		{
			return 0;
		}

		if(m_oRoleSet.find(poRole->GetID()) != m_oRoleSet.end())
		{
			return 0;
		}

		oReq.m_oRoleList.push_back(poRole->GetID());
		oReq.m_oWaitSet.insert(poRole->GetID());
	}

	++m_dwSerialID;
	if(m_dwSerialID == 0)
	{
		m_dwSerialID = 1;
	}

	oReq.m_dwSerialID = m_dwSerialID;
	oReq.m_dwRpcDelayed = dwRpcDealyed;
	oReq.m_qwBeginTime = TimeUtil::GetMilliSecond();
	m_oReqMap.insert(std::make_pair(oReq.m_dwSerialID, oReq));
	return oReq.m_dwSerialID;
}

void CLeaveSceneControl::CancelLeave(UINT32 dwSerialID)
{
	CRequestMap::iterator it = m_oReqMap.find(dwSerialID);
	if(it != m_oReqMap.end())
	{
		m_oReqMap.erase(it);
	}
}

CLeaveSceneReq* CLeaveSceneControl::GetReq(UINT32 dwSerialID)
{
	CRequestMap::iterator it = m_oReqMap.find(dwSerialID);
	return it == m_oReqMap.end() ? NULL : &it->second;
}

void CLeaveSceneControl::OnSaveRoleDataReply(UINT32 dwSerialID, UINT64 qwRoleID, KKSG::ErrorCode nResult)
{
	CLeaveSceneReq* poReq = GetReq(dwSerialID);
	if(poReq == NULL)
	{
		LogWarn("Can not find request %u when save data to db reply", dwSerialID);
		return;
	}

	if(nResult != KKSG::ERR_SUCCESS)
	{
		LogDebug("Leave scene save data failed, role: %llu, error: %d, can't leave scene", qwRoleID, nResult);

		CUserData oUser;
		oUser.m_dwUserData = (UINT32)nResult;
		CRpc::ReplyDelayRpc(poReq->m_dwRpcDelayed, oUser);
		m_oReqMap.erase(dwSerialID);
		return;
	}

	poReq->m_oWaitSet.erase(qwRoleID);
	if(poReq->m_oWaitSet.empty())
	{
		poReq->m_qwEndTime = TimeUtil::GetMilliSecond();
		UINT32 dwTimeUsed = (UINT32)(poReq->m_qwEndTime >= poReq->m_qwBeginTime ? poReq->m_qwEndTime - poReq->m_qwBeginTime : 0);
		LogDebug("Leave scene save data use time: %u ms", dwTimeUsed);

		CUserData oUser;
		oUser.m_dwUserData = (UINT32)KKSG::ERR_SUCCESS;
		oUser.m_pUserPtr = poReq;
		CRpc::ReplyDelayRpc(poReq->m_dwRpcDelayed, oUser);
		m_oReqMap.erase(dwSerialID);
	}
}