#include "pch.h"
#include "common/rpct2r_registergt2router.h"

// generate by ProtoGen at date: 2016/10/27 9:57:39

RPC_CLIENT_IMPLEMETION(RpcT2R_RegisterGt2Router, RegisterGt2RouterArg, RegisterGt2RouterRes)

void RpcT2R_RegisterGt2Router::OnReply(const RegisterGt2RouterArg &roArg, const RegisterGt2RouterRes &roRes, const CUserData &roUserData)
{
	SSInfo << "Register To Router, code: " << roRes.result() << END;
}

void RpcT2R_RegisterGt2Router::OnTimeout(const RegisterGt2RouterArg &roArg, const CUserData &roUserData)
{
	LogError("Register To Router Timeout");
}
