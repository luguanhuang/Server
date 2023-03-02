#include "pch.h"
#include "pushsubscribe.h"
#include "config.h"
#include "util/jsonutil.h"
#include "appid.h"
#include "HashUtil.h"

CPushSubscribeTask::CPushSubscribeTask()
{
	m_strUri = "/profile/subscribe_push";
}

CPushSubscribeTask::~CPushSubscribeTask()
{

}

void CPushSubscribeTask::OnResult(const std::string &response)
{
	SSInfo << response << END;
	Print();
}

void CPushSubscribeTask::OnFailed()
{
	Print();
}

void CPushSubscribeTask::BuildPostData(UINT32 iMsgId)
{
	JsonBuilder oBuilder;
	oBuilder.AddMember_String("appid", m_strAppid);
	oBuilder.AddMember_String("openid", m_strOpenId);
	oBuilder.AddMember_Int("msgId", iMsgId);
	std::string str = oBuilder.GetString();
	SSDebug << str << END;
	SetHttpPostString(str);
}

void CPushSubscribeTask::Print()
{
	SSInfo << "pushurl=" << m_strUrl << END;
}