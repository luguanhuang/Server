#include "pch.h"
#include "login/ptcn2i_usedlogintokennotify.h"
#include "verify/tokenverifymgr.h"
#include "verify/DBUpdateLastServerTask.h"

// generate by ProtoGen at date: 2016/11/3 10:26:25

void PtcN2I_UsedLoginTokenNotify::Process(UINT32 dwConnID)
{
	UToken token;
	if (!UToken::FromString(token, m_Data.logintoken().data(), m_Data.logintoken().size()))
	{
		LogWarn("remove logintoken error, size %d", m_Data.logintoken().size());
		return;
	}

	char buf[64];
	token.Format(buf, 64);

	UINT32 serverID = LoginConfig::Instance()->GetServerID();
	if (token.data[4] != serverID)
	{
		LogWarn("send logintoken to wrong loginserver: token[%s] serverID: %d, here serverID: %d", 
			buf, token.data[4], serverID);
		return;
	}

	UserInfo *pUser = TokenVerifyMgr::Instance()->FindUserInfo(token);
	if (pUser == NULL)
	{
		LogWarn("not found token[%s]", buf);
		return;
	}

	if (pUser->isused)
	{
		LogWarn("token[%s] already used", buf);
		return;
	}

	LogInfo("remove token[%s],  account[%s]", buf, pUser->account.c_str());
	pUser->isused = true;

	DBUpdateLastServerTask *pTask = new DBUpdateLastServerTask(pUser->account, m_Data.serverid());
	TokenVerifyMgr::Instance()->AddDBTask(pTask);
}
