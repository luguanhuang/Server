#include "pch.h"
#include "accountkick.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "network/controllink.h"
#include "idip/idipproc.h"
#include "login/rpcm2n_kickaccountfromms.h"

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

bool CAccountKicker::KickAccount(UINT64 qwRoleID, KKSG::KickType nKickType, IAccountKickListener* poListener, INT32 dwID)
{
	if(qwRoleID == INVALID_ROLE_ID) return false;

	CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
	if(poRole == NULL)
	{
		LogDebug("Role %llu is not online", qwRoleID);
		return false;
	}

	if(poRole->GetState() != ROLE_STATE_IN_GAME)
	{
		LogError("Role %llu is not in game, retry later", qwRoleID);
		return false;
	}

	if(m_oMap.find(poRole->GetAccount()) != m_oMap.end())
	{
		LogError("Role %llu is kicking ...", qwRoleID);
		return false;
	}

	AccountKickInfo& roInfo = m_oMap[poRole->GetAccount()];
	roInfo.m_qwRoleID = qwRoleID;
	roInfo.m_nKickType = nKickType;
	roInfo.m_poListener = poListener;
	roInfo.dwID = dwID;

	RpcM2N_KickAccountFromMs* rpc = RpcM2N_KickAccountFromMs::CreateRpc();
	rpc->m_oArg.set_roleid(poRole->GetID());
	rpc->m_oArg.set_accountid(poRole->GetAccount());
	rpc->m_oArg.set_kicktype(nKickType);
	ControlLink::Instance()->SendTo(*rpc);

	LogInfo("Begin kick role=[%llu], account=[%s]", qwRoleID, poRole->GetAccount().c_str());
	return true;
}

void CAccountKicker::OnKickReply(const std::string& strAccount, KKSG::ErrorCode nErrCode)
{
	CKickInfoMap::iterator it = m_oMap.find(strAccount);
	if(it == m_oMap.end())
	{
		return;
	}

	AccountKickInfo& roInfo = it->second;
	if(roInfo.m_poListener != NULL)
	{
		roInfo.m_poListener->OnKickReply(nErrCode, strAccount, roInfo.m_qwRoleID);
	}

	if(roInfo.dwID)//idip功能需要替下线的，踢完开始操作
	{
		if (!CIdipProcess::Instance()->KickoutHandler(roInfo.dwID))
		{
			CUserData oData;
			oData.m_dwUserData = IDIP_ERR_KICKOUT;
			CIdipProcess::Instance()->OnAsyncReply(roInfo.dwID, oData);
		}
	}
	m_oMap.erase(it);
}