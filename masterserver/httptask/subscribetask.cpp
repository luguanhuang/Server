#include "pch.h"
#include "subscribetask.h"
#include "config.h"
#include "crpc.h"
#include "util/jsonutil.h"
#include "appid.h"
#include "push/pushsubscribemgr.h"
#include "util/gametime.h"
#include "HashUtil.h"

SubscribeTask::SubscribeTask()
{
	m_rpcId = 0;
	SetType(true);
	m_strAppid = WeChat_App_ID;//MSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID;
	m_strAppKey = WeChat_App_Key;//MSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_Key:WeChat_App_Key;
}

SubscribeTask::~SubscribeTask()
{

}

const std::string SubscribeTask::GetUrl()
{
	std::ostringstream ss;
	UINT32 ts = UINT32(time(NULL));
	std::string sig = MakeSign(ts, m_strAppKey);
	ss << MSConfig::Instance()->GetMsdkUrl() << m_strUri << "?timestamp=" << ts
		<< "&appid=" << m_strAppid << "&sig=" << sig << "&openid=" << m_strOpenId << "&encode=1";
	//SSDebug << ss.str() << END;
	m_strUrl = ss.str();
	return ss.str();
}

std::string SubscribeTask::MakeSign(UINT32 ts, std::string key)
{
	std::ostringstream ss;
	ss << key << ts;
	//SSDebug << "MakeSign=" << ss.str() << END;
	return Md5Hash(ss.str());
}

