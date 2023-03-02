#include "pch.h"
#include "querywebchatsharechestid.h"
#include "config.h"
#include "globalconfig.h"
#include "appid.h"
#include "HashUtil.h"
#include "util/gametime.h"

const std::string web_chat_key = "BGYYm9Bf8fgT9giqFMNH2rR7pRLUYUaP";
const UINT32 web_chat_type = 3;
const std::string web_chat_act_id = "hongbaocard_3505_3248373643";

QueryWebChatShareChestid::QueryWebChatShareChestid()
	:rpc_id_(0)
{
	SetType(false);
	SetTimeOut(3000);

	rpc_id_ = 0;
	open_id_.clear();	
	bussid_= "0";
}

QueryWebChatShareChestid::~QueryWebChatShareChestid()
{

}

const std::string QueryWebChatShareChestid::GetUrl()
{
	time_t now = GameTime::GetTime();
	bussid_ = ToString(GameTime::GetDayUnique(now));
	std::ostringstream oss;
	oss << GSConfig::Instance()->GetGameWeiXinUrl() << "?actid=" << web_chat_act_id
		<< "&bussid=" << bussid_ 
		<< "&openid=" << open_id_
		<<"&stamp="<<now
		<<"&type="<<web_chat_type
		<<"&sign="<<GetSign((UINT32)now);
	
	LogDebug("%s",oss.str().c_str());
	return oss.str();
}

void QueryWebChatShareChestid::OnResult(const std::string &response)
{
	LogDebug("get tencent response result: %s", response.c_str());

	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		LogError("parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}

	if (!doc.HasMember("ret"))
	{
		LogError("parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}
	int ret = doc["ret"].GetInt();
	if (ret != 0)
	{		
		LogWarn(" parser json failed, %s", response.c_str());
		OnFailed();
		return;
	}

	if (!doc.HasMember("data"))
	{
		OnFailed();
		return;
	}
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(doc, "data");
	if(roMember.IsNull()) 
	{
		OnFailed();
		return;
	}

	string url = JsonUtil::GetJsonString(roMember,"url");
	if (url.length() > 0)
	{
		KKSG::GetPlatformShareChestRes res;
		res.set_errorcode(KKSG::ErrorCode(ret));
		res.set_url(url);
		if (rpc_id_ > 0)
		{
			CUserData oUserData;
			oUserData.m_dwUserData = 0;
			oUserData.m_pUserPtr = (void*)(&res);
			CRpc::ReplyDelayRpc(rpc_id_, oUserData);
		}
	}
}

void QueryWebChatShareChestid::OnFailed()
{
	SSWarn << "query QueryPlatformShareChestid failed, openId = " << open_id_ <<  END; 

	KKSG::GetPlatformShareChestRes res;
	res.set_errorcode(KKSG::ERR_PLATSHARE_FAILED);
	if (rpc_id_ > 0)
	{
		CUserData oUserData;
		oUserData.m_dwUserData = 0;
		oUserData.m_pUserPtr = &res;
		CRpc::ReplyDelayRpc(rpc_id_, oUserData);
	}
}

std::string QueryWebChatShareChestid::GetSign(UINT32 now)
{
	/*sign=MD5(actid=变量&bussid=变量&openid=变量&stamp=变量&type=变量&key=密钥)*/
	std::ostringstream oss;
	oss << "actid="<<web_chat_act_id
		<<"&bussid="<<bussid_
		<<"&openid="<<open_id_
		<<"&stamp="<<now
		<<"&type="<<web_chat_type
		<<"&key="<<web_chat_key;
	return Md5Hash(oss.str());
}


