#include "pch.h"
#include "gm/rpcp2m_gmhttpcommandnew.h"
#include "gm/rpcm2g_gshttpcommandnew.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "util.h"
#include "util/jsonutil.h"
#include "idip/idiphandler.h"
#include "idip/idiperrdesc.h"
#include "idip/idipproc.h"
#include "account/punishmgr.h"
#include "account/accountkick.h"
#include "gm/rpcm2i_getplatnoticeinfo.h"
#include "network/loginlink.h"
#include "config.h"
#include "pay/paymgr.h"
#include "pay/SnsSigCheck.h"
#include "pay/ibgiftmgr.h"
// generate by ProtoGen at date: 2016/7/16 10:19:30

RPC_SERVER_IMPLEMETION(RpcP2M_GMHttpCommandNew, GMHttpArg, GMHttpRes)

void RpcP2M_GMHttpCommandNew::OnCall(const GMHttpArg &roArg, GMHttpRes &roRes)
{
	if (roArg.command() == "/idip")	//temp
	{
		std::map<std::string, std::string> values;
		for (int i = 0; i < roArg.keys_size(); ++i)
		{
			values[roArg.keys(i)] = roArg.values(i);
		}

		auto itera = values.find("data_packet");

		if (itera == values.end())
		{
			roRes.set_result(400);
			roRes.set_resultstring(roArg.command() + " para error !");
			LogInfo("request para error ", roArg.command().c_str());
			return ;
		}
		CIdipReply oReply(*roRes.mutable_resultstring());
		GMHttpArg Arg = roArg; 
		CIdipHandler* poAsyncHandler = CIdipProcess::Instance()->Process(roArg.values(0), oReply, roArg);
		if(poAsyncHandler == NULL)
		{
			roRes.set_result(oReply.m_nErrCode);
			roRes.set_resultstring(oReply.m_strResult);
		}
		else if (poAsyncHandler->GetServerType() == IDIP_SERVER_NONE)//处理完直接返回
		{
			roRes.set_result(oReply.m_nErrCode);
			roRes.set_resultstring(oReply.m_strResult);
			CIdipProcess::Instance()->Release(poAsyncHandler);
		}
		else
		{
			poAsyncHandler->SetRpcID(DelayRpc());
			if (IDIP_SERVER_GS ==  poAsyncHandler->GetServerType())
			{
				RpcM2G_GSHttpCommandNew* cmd = RpcM2G_GSHttpCommandNew::CreateRpc();
				cmd->m_oArg.CopyFrom(roArg);
				///> use field httpid as delay rpc id
				cmd->m_oArg.set_httpid(poAsyncHandler->GetID());
				if (poAsyncHandler->GetRoleID())
				{
					CRole* pRole = CRoleManager::Instance()->GetByRoleID(poAsyncHandler->GetRoleID());
					if (NULL != pRole)
					{
						pRole->SendMsgToGS(*cmd);
						oReply.m_nErrCode = IDIP_ERR_SUCCESS;
					}
				}
				else
				{
					GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *cmd);
				}
			}
			else if (IDIP_SERVER_MS == poAsyncHandler->GetServerType())
			{
				if (!poAsyncHandler->IsKickAccount())//非踢下线状态
				{
					//去db读取数据的临时锁住角色，禁止登录;
					if (poAsyncHandler->ReadDataQequest() && poAsyncHandler->GetRoleID())
					{
						CPunishMgr::Instance()->DoPunishTemp(poAsyncHandler->GetRoleID());
					}
				}
			}
			else if (IDIP_SERVER_LOGIN == poAsyncHandler->GetServerType())
			{
				RpcM2I_GetPlatNoticeInfo* cmd = RpcM2I_GetPlatNoticeInfo::CreateRpc();
				cmd->m_oArg.set_id(poAsyncHandler->GetID());
				LoginLink::Instance()->SendTo(*cmd);
			}
			else if (IDIP_SERVER_OTHER == poAsyncHandler->GetServerType())
			{
				if (!poAsyncHandler->DoOther(oReply))
				{
					roRes.set_result(oReply.m_nErrCode);
					roRes.set_resultstring(PackErrResponse(oReply.m_nErrCode));
					CIdipProcess::Instance()->Release(poAsyncHandler);
				}
			}
			CIdipProcess::Instance()->AddHandler(poAsyncHandler);
		}
	}
	else if (roArg.command() == "/buynotify")
	{
		std::map<std::string, std::string> params;
		std::string sig;
		std::ostringstream ss;
		std::string account;
		for (int i = 0; i < roArg.keys_size(); ++i)
		{
			if (roArg.keys(i) == "sig")
			{
				sig = UrlDecode(roArg.values(i));
			}
			else
			{
				params[roArg.keys(i)] = roArg.values(i);
			}
			ss << roArg.keys(i) << "=" << roArg.values(i) << ","; 
			if (roArg.keys(i) == "openid")
			{
				account = roArg.values(i);
			}
			//SSInfo << "key=" << roArg.keys(i) << ",value=" << roArg.values(i) << END;
		}
		std::ostringstream msg;
		INT32 ret = IBGiftMgr::Instance()->PayBuyGoods(params, sig, msg);
		roRes.set_result(ret);
		roRes.set_resultstring(IBGiftMgr::Instance()->PayBuyGoodsRes(ret, msg.str()));
		if (ret)
		{
			SSWarn << "openid=" << account << "," << msg.str() << "," << ss.str() << END;
		}
		else
		{
			SSInfo << ss.str() << END;
		}
	}
	else
	{
		roRes.set_result(404);
		roRes.set_resultstring(roArg.command() + " is not support !");

		LogInfo("command %s is not support ", roArg.command().c_str());
	}
}

void RpcP2M_GMHttpCommandNew::OnDelayReplyRpc(const GMHttpArg &roArg, GMHttpRes &roRes, const CUserData &roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		roRes.set_result(IDIP_ERR_UNKNOWN);
		roRes.set_errormsg(GetIdipErrDesc(IDIP_ERR_UNKNOWN));
	}
	else
	{
		CIdipReply* poReply = (CIdipReply*)roUserData.m_pUserPtr;
		roRes.set_resultstring(poReply->m_strResult);
		roRes.set_result(poReply->m_nErrCode);
		roRes.set_errormsg(GetIdipErrDesc(poReply->m_nErrCode));
	}
}
