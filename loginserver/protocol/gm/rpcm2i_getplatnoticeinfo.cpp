#include "pch.h"
#include "gm/rpcm2i_getplatnoticeinfo.h"
#include "notice/platnoticemgr.h"

// generate by ProtoGen at date: 2016/11/19 14:54:48

RPC_SERVER_IMPLEMETION(RpcM2I_GetPlatNoticeInfo, GetPlatNoticeArg, GetPlatNoticeRes)

void RpcM2I_GetPlatNoticeInfo::OnCall(const GetPlatNoticeArg &roArg, GetPlatNoticeRes &roRes)
{
	roRes.set_result(ERR_SUCCESS);
	CPlatNoticeInfoMap olist = CPlatNoticeMgr::Instance()->GetNoticeList();
	for (auto i = olist.begin(); i != olist.end(); i ++)
	{
		KKSG::PlatNotice* pNotice =  roRes.add_noticelist();
		pNotice->CopyFrom(i->second);
	}
}

void RpcM2I_GetPlatNoticeInfo::OnDelayReplyRpc(const GetPlatNoticeArg &roArg, GetPlatNoticeRes &roRes, const CUserData &roUserData)
{
}
