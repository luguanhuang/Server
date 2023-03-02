#include "pch.h"
#include <time.h>
#include "login/rpcc2i_querygateip.h"
#include "verify/tokenverifymgr.h"
#include "verify/SNDALoginTask.h"
#include "verify/TXLoginTask.h"
#include "verify/DBCheckPasswordTask.h"
#include "gatemgr/gatemgr.h"
#include "verify/logincontrol.h"
#include "iplist/whitelistmgr.h"
#include "iplist/accountbanmgr.h"
#include "notice/platnoticemgr.h"

// generate by ProtoGen at date: 2015/9/16 15:23:58

RPC_SERVER_IMPLEMETION(RpcC2I_QueryGateIP, QueryGateArg, QueryGateRes)



void RpcC2I_QueryGateIP::OnCall(const QueryGateArg &roArg, QueryGateRes &roRes)
{
	roRes.set_logintoken("");
	roRes.set_userphone("");

	if(!CLoginControl::Instance()->CheckLogin(roArg.openid()))
	{
		return;
	}

	UINT32 dwIP;
	UINT16 wPort;
	CNetProcessor::Instance()->GetRemoteAddr(m_dwConnID, dwIP, wPort);

	//检查是否封号.
	
	if(AccountBanMgr::Instance()->IsOpenIDInBanList(roArg.openid())==true)
	{
		roRes.set_error(KKSG::ERR_PLAT_BANACC);
		KKSG::PlatBanAccount *pBanInfo = roRes.mutable_baninfo();
		AccountBanMgr::Instance()->FillReason(roArg.openid(),pBanInfo);
		return;
	}
	if (roArg.type() == KKSG::LOGIN_PASSWORD)
	{
		LogInfo("client query gate [%s - %s]", roArg.account().c_str(), roArg.password().c_str());
			
		if (roArg.account().empty() || roArg.password().empty())
		{
			return;
		}

		LoginInfo info;
		info.ip =  dwIP;
		info.delayRpcID = DelayRpc();
		info.openID = roArg.account();
		info.loginType = roArg.type();
		info.Init();

		DBCheckPasswordTask *pTask = new DBCheckPasswordTask();
		pTask->SetAccount(info.openID);
		pTask->SetPassword(roArg.password());
		pTask->SetRpcID(info.delayRpcID);
		pTask->SetIP(info.ip);
		CLoginControl::Instance()->BeginLogin(/*roArg.openid()*/info);
		TokenVerifyMgr::Instance()->AddDBTask(pTask);
	}
	else if (roArg.type() == KKSG::LOGIN_SNDA_PF)
	{
		LogInfo("client query gate use token [%s]", roArg.token().c_str());

		if (roArg.token().empty())
		{
			return;
		}

		SNDALoginTask *pTask = new SNDALoginTask();
		pTask->SetRpcID(DelayRpc());
		pTask->SetToken(roArg.token());
		//pTask->SetPF(roArg.pf());
		pTask->SetIP(dwIP);
		TokenVerifyMgr::Instance()->AddPFVerifyTask(pTask);
	}
	else if(roArg.type() == KKSG::LOGIN_QQ_PF || roArg.type() == KKSG::LGOIN_WECHAT_PF || roArg.type() == KKSG::LOGIN_IOS_GUEST)
	{
		LogInfo("client query gate use token [%s], openid [%s]", roArg.token().c_str(), roArg.openid().c_str());
		if (roArg.token().empty() || roArg.openid().empty())
		{
			return;
		}

		LoginInfo info;
		info.ip =  dwIP;
		info.delayRpcID = DelayRpc();
		info.tencentAuthToken = roArg.token();
		info.openID = roArg.openid();
		info.loginType = roArg.type();
		info.Init();

		TXLoginTask* pTask = new TXLoginTask();
		pTask->SetIP(info.ip);
		pTask->SetRpcID(info.delayRpcID);
		pTask->SetToken(info.tencentAuthToken);
		pTask->SetOpenId(info.openID);
		pTask->SetLoginType(info.loginType);
		pTask->BuildPostData();
		CLoginControl::Instance()->BeginLogin(/*roArg.openid()*/info);
		TokenVerifyMgr::Instance()->AddPFVerifyTask(pTask);
	}
	else
	{
		LogWarn("invalid login type [%d]", roArg.type());
	}
}

// 有角色玩家就是最近登录的服，没角色玩家设置一个推荐服
static void SetRecommandGateInfo(GateInfo* pInfo, UserGateInfo* pUserInfo, QueryGateRes &roRes)
{
	if (pInfo == NULL)
	{
		return;
	}
	if (pUserInfo == NULL)
	{
		return;
	}
	pInfo->FillGateInfo(pUserInfo->ccType, pUserInfo->isFreeFlow, *roRes.mutable_recommandgate());
	LogInfo("query gate return recommand server[%s:%d]", roRes.recommandgate().ip().c_str(), roRes.recommandgate().port());
}

static void SetSelfServerInfo(QueryGateRes &roRes, std::map<UINT32, UINT32>& serverid2level, KKSG::LoginType loginType, bool isInWhiteList, UserGateInfo* pUseInfo)
{
	for (auto i = serverid2level.begin(); i != serverid2level.end(); ++i)
	{
		GateInfo *pInfo = GateMgr::Instance()->GetServerInfo(i->first);
		if (pInfo == NULL)
		{
			continue;
		}
		if (!pInfo->CanSee(loginType, isInWhiteList))
		{
			continue;
		}
		KKSG::SelfServerData* data = roRes.add_servers();
		pInfo->FillGateInfo(pUseInfo->ccType, pUseInfo->isFreeFlow, *data->mutable_servers());
		data->set_level(i->second);
	}
}

void RpcC2I_QueryGateIP::OnDelayReplyRpc(const QueryGateArg &roArg, QueryGateRes &roRes, const CUserData &roUserData)
{
	roRes.set_logintoken("");
	roRes.set_userphone("");
	roRes.set_loginzoneid(LoginConfig::Instance()->GetServerID());

	if (roUserData.m_dwUserData != 1 || roUserData.m_pUserPtr == NULL)
	{
		LogInfo("gate query reply failed, openId[%s]", roArg.openid().c_str());
		return;
	}

	UserGateInfo *pUserInfo = (UserGateInfo *)roUserData.m_pUserPtr; 
	bool isInWhiteList = WhiteListMgr::Instance()->IsOpenIDInWhiteList(roArg.openid());
	KKSG::LoginType type = roArg.type();

	// 苹果审核版本需要进审核服(推荐服和区服列表都要给审核服的)
	const std::string& strIosAuditVersion = LoginConfig::Instance()->GetIosAuditVersion();
	if(roArg.platid() == KKSG::PLAT_IOS && !strIosAuditVersion.empty() && strIosAuditVersion.compare(roArg.version()) == 0)
	{
		type = KKSG::LOGIN_IOS_AUDIT;
		LogInfo("Account [%s] version [%s], fetch ios audit server list", roArg.account().c_str(), roArg.version().c_str());
	}

	GateInfo* pInfo = GateMgr::Instance()->GetRecommendServerInfo(pUserInfo, type, isInWhiteList);
	if (pInfo == NULL)
	{
		CLoginControl::Instance()->EndLogin(roArg.account());
		SSWarn << "No recommend server, Maybe no server id:" << pUserInfo->serverID << " not find" << END;
		return;
	}

	roRes.set_logintoken(pUserInfo->uid.str(), pUserInfo->uid.size());
	roRes.set_userphone(pUserInfo->phone);
	roRes.set_freeflow(pUserInfo->isFreeFlow);
	roRes.set_cctype(pUserInfo->ccType);
	SetRecommandGateInfo(pInfo, pUserInfo, roRes);
	SetSelfServerInfo(roRes, pUserInfo->serverid2level, type, isInWhiteList, pUserInfo);

	GateMgr::Instance()->FillDummyRecommendServerInfo(type, isInWhiteList, roRes, pUserInfo);
	GateMgr::Instance()->FillServerGateInfo(type, isInWhiteList, roRes, pUserInfo);
	GateMgr::Instance()->FillRecommendServerInfo(type, isInWhiteList, roRes, pUserInfo);

	roRes.set_in_white_list(isInWhiteList);
	KKSG::PlatNotice* notice = CPlatNoticeMgr::Instance()->GetNotice(roArg.type(), roArg.platid());
	if (notice)
	{
		roRes.mutable_notice()->CopyFrom(*notice);
	}

	for (auto iter = pUserInfo->friendServer.begin(); iter != pUserInfo->friendServer.end(); ++iter)
	{
		roRes.add_platfriendservers()->CopyFrom(*iter);
	}
	for (auto it = pUserInfo->bespeakServerIds.begin(); it != pUserInfo->bespeakServerIds.end(); ++it)
	{
		roRes.add_bespeakserverids(*it);
	}
}
