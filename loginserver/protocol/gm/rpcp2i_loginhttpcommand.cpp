#include "pch.h"
#include "gm/rpcp2i_loginhttpcommand.h"
#include "gmmgr/gmmgr.h"
#include "sql/mysqlmgr.h"
#include "idip/idiphandler.h"
#include "util/jsonutil.h"
#include "idip/idiperrdesc.h"
#include "gatemgr/gatemgr.h"

// generate by ProtoGen at date: 2016/2/22 16:51:54

RPC_SERVER_IMPLEMETION(RpcP2I_LoginHttpCommand, GMHttpArg, GMHttpRes)

void RpcP2I_LoginHttpCommand::OnCall(const GMHttpArg &roArg, GMHttpRes &roRes)
{
	LogInfo("exe command %s ", roArg.command().c_str());

	std::string op;
	std::map<std::string, std::string> keyvalues;
	for(auto i = 0; i < roArg.keys_size() && i < roArg.values_size(); ++i)
	{
		LogInfo("gm key=value: %s = %s",roArg.keys(i).c_str(),roArg.values(i).c_str());
		keyvalues.insert(std::pair<std::string, std::string>(roArg.keys(i), roArg.values(i)));
		if (roArg.keys(i) == "op")
		{
			op = roArg.values(i);
		}
	}

	//if (roArg.command() == "/whitelist")
	//{
	//	std::string resultstring;
	//	if ("query" == op)
	//	{
	//		GMMgr::Instance()->HttpQuery(keyvalues, resultstring);	
	//	}
	//	else if ("add" == op)
	//	{
	//		GMMgr::Instance()->HttpAdd(keyvalues, resultstring);	
	//	}
	//	else if ("del" == op)
	//	{
	//		GMMgr::Instance()->HttpDel(keyvalues, resultstring);	
	//	}
	//	roRes.set_result(200);
	//	roRes.set_resultstring(resultstring);
	//}
	if (roArg.command() == "/idip")
	{
		INT32 result = IDIP_ERR_UNKNOWN;
		std::string resultstring;
		if ("idipadd" == op)
		{
			GMMgr::Instance()->IdipAdd(keyvalues, result);
			resultstring = PackErrResponse(EIdipErrCode(result));
		}
		else if (op ==  "dobespeak")
		{
			auto info = GateMgr::Instance()->GetServerInfo(roArg.serverid());
			if (info == NULL)
			{
				result = IDIP_ERR_INVALID_SERVERID;
			}
			else if (MysqlMgr::Instance()->InsertBespeakAccount(keyvalues, roArg.serverid()))
			{
				result = IDIP_ERR_SUCCESS;
			}
			resultstring = PackErrResponse(EIdipErrCode(result));
		}
		else if (op == "querybespeak")
		{
			std::vector<UINT32> vlist;
			if (MysqlMgr::Instance()->QueryBespeakList(keyvalues, vlist))
			{
				result = IDIP_ERR_SUCCESS;
				JsonBuilder Builder;
				Builder.AddMember_Int("Result", result);
				Builder.AddMember_String("RetMsg", GetIdipErrDesc(EIdipErrCode(result)).c_str());
				rapidjson::Value oList(rapidjson::kArrayType);
				for (UINT32 i = 0; i < vlist.size(); i ++)
				{
					rapidjson::Value listinfo(rapidjson::kObjectType);
					listinfo.AddMember("Partition", vlist[i], Builder.Allocator());
					std::string strSvrName;
					auto info = GateMgr::Instance()->GetServerInfo(vlist[i]);
					if (info!=NULL)
					{
						strSvrName = info->serverName;
					}
					listinfo.AddMember("PartitionName", UrlEncode(strSvrName), Builder.Allocator());
					oList.PushBack(listinfo, Builder.Allocator());
				}
				Builder.AddMember_Uint("PartitionList_count", vlist.size());
				Builder.AddMember_Value("PartitionList", oList);
				resultstring = Builder.GetString();
			}
			else
			{
				resultstring = PackErrResponse(EIdipErrCode(result));
			}
		}
		roRes.set_result(result);
		roRes.set_resultstring(resultstring);
	}
	else
	{
		roRes.set_result(404);
		roRes.set_resultstring(roArg.command() + " is not support !");

		LogWarn("command %s is not support ", roArg.command().c_str());
	}
}

void RpcP2I_LoginHttpCommand::OnDelayReplyRpc(const GMHttpArg &roArg, GMHttpRes &roRes, const CUserData &roUserData)
{
}
