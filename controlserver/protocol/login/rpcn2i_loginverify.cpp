#include "pch.h"
#include "login/rpcn2i_loginverify.h"
#include "account/logincontrol.h"

// generate by ProtoGen at date: 2016/11/2 20:14:40

RPC_CLIENT_IMPLEMETION(RpcN2I_LoginVerify, LoginVerifyArg, LoginVerifyRes)

void RpcN2I_LoginVerify::OnReply(const LoginVerifyArg &roArg, const LoginVerifyRes &roRes, const CUserData &roUserData)
{
	CLoginControl::Instance()->OnTokenVerfiyCompleted(roArg.uid(), roRes.userid(),  roRes.result(), &roRes);
}

void RpcN2I_LoginVerify::OnTimeout(const LoginVerifyArg &roArg, const CUserData &roUserData)
{
	CLoginControl::Instance()->OnTokenVerfiyCompleted(roArg.uid(), "", KKSG::ERR_TIMEOUT, NULL);
}
