#include "pch.h"
#include "ordercreate.h"
#include "config.h"
#include "crpc.h"
#include <time.h>


OrderCreateTask::OrderCreateTask()
{
	m_rpcId = 0;
	SetType(true);
	SetTimeOut(3000);
	m_strUrl = MSConfig::Instance()->GetMsdkUrl();
}

OrderCreateTask::~OrderCreateTask()
{

}

const std::string OrderCreateTask::GetUrl()
{
	std::ostringstream ss;
	ss << m_strUrl << "/?service=Order.Create";
	return ss.str();
}

void OrderCreateTask::OnResult(const std::string &response)
{
	SSDebug << response << END;
	CUserData data;
	data.m_dwUserData = 0;
	data.m_pUserPtr = (void*)&response;
	CRpc::ReplyDelayRpc(m_rpcId, data);
}

void OrderCreateTask::OnFailed()
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_UNKNOWN;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(m_rpcId, data);
	SSDebug << "failed" << END;
}

/*void OrderCreateTask::BuildPostData(KKSG::OrderCreateArg oData)
{
	std::ostringstream ss;
	ss << "pf=" << oData.pf() 
	   << "&area=" << oData.area()
	   << "&uin=" << oData.uin()
	   << "&uid=" << oData.uid()
	   << "&req=" << oData.req()
	   << "&time=" << INT32(time(NULL));
	SSDebug << ss.str() << END;
	SetHttpPostString(ss.str());
}
*/