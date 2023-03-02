#include "pch.h"
#include "accountkick.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "scene/rpcn2g_leavescene.h"


INSTANCE_SINGLETON(CAccountKicker)


CAccountKicker::CAccountKicker()
{
}

CAccountKicker::~CAccountKicker()
{
}

bool CAccountKicker::Init()
{
	return true;
}

void CAccountKicker::Uninit()
{
}

KKSG::ErrorCode CAccountKicker::KickAccount(UINT64 qwRoleID, KKSG::KickType nKickType, AccountKickInfo** ppKickInfo)
{
	if(qwRoleID == INVALID_ROLE_ID) return KKSG::ERR_FAILED;

	CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
	if(poRole == NULL)
	{
		LogDebug("Role %llu is not online", qwRoleID);
		return KKSG::ERR_ROLE_NOT_ONLINE;
	}

	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		LogError("Role %llu is change scene, retry later", qwRoleID);
		return KKSG::ERR_STATE_ERROR;
	}

	if(m_oMap.find(poRole->GetAccount()) != m_oMap.end())
	{
		LogError("Role %llu is kicking ...", qwRoleID);
		return KKSG::ERR_FAILED;
	}

	poRole->SetState(ROLE_STATE_LEAVE_SCENE);
	AccountKickInfo& roInfo = m_oMap[poRole->GetAccount()];
	roInfo.m_qwRoleID = qwRoleID;
	roInfo.m_nKickType = nKickType;

	//要考虑正在切场景的
	RpcN2G_LeaveScene* rpc = RpcN2G_LeaveScene::CreateRpc();
	rpc->m_oArg.add_rolelist(poRole->GetID());
	rpc->m_oArg.set_type(KKSG::LEAVE_SCENE_LOGOUT);
	rpc->m_oArg.set_logouttype(nKickType == KKSG::KICK_CHANGE_PROFESSION ? KKSG::LOGOUT_CHANGEPROFESSION : KKSG::LOGOUT_IDIP_KICK_ACCOUNT);
	rpc->m_oArg.set_account(poRole->GetAccount());
	rpc->m_oArg.set_line(poRole->GetGsLine());
	poRole->SendMsgToGS(*rpc);
	LogInfo("Begin kick role=[%llu], account=[%s]", qwRoleID, poRole->GetAccount().c_str());

	if(ppKickInfo != NULL)
	{
		*ppKickInfo = &roInfo;
	}
	return KKSG::ERR_SUCCESS;
}

void CAccountKicker::OnKickReply(const std::string& strAccount, KKSG::ErrorCode nErrCode, KKSG::KickType& nKickType)
{
	CKickInfoMap::iterator it = m_oMap.find(strAccount);
	if(it == m_oMap.end())
	{
		return;
	}

	AccountKickInfo& roInfo = it->second;
	nKickType = roInfo.m_nKickType;

	//reply to ms
	CUserData oUser((UINT32)nErrCode, NULL);
	CRpc::ReplyDelayRpc(roInfo.m_dwRpcDelayed, oUser);

	//log
	LogInfo("End kick role=[%llu], account=[%s], result=[%d], kicktype=[%d]", roInfo.m_qwRoleID, strAccount.c_str(), nErrCode, nKickType);
	m_oMap.erase(it);
}