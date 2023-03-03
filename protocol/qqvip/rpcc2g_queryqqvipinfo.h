#ifndef __RPCC2G_QUERYQQVIPINFO_H__
#define __RPCC2G_QUERYQQVIPINFO_H__

// generate by ProtoGen at date: 2016/11/20 22:23:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYQQVIPINFO_TYPE 43943

class RpcC2G_QueryQQVipInfo : public CRpc
{
private:

	RpcC2G_QueryQQVipInfo():CRpc(RPCC2G_QUERYQQVIPINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryQQVipInfo *CreateRpc()
	{
		return new RpcC2G_QueryQQVipInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryQQVipInfoArg &roArg, const QueryQQVipInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryQQVipInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryQQVipInfoArg &roArg, QueryQQVipInfoRes &roRes);
	void OnDelayReplyRpc(const QueryQQVipInfoArg &roArg, QueryQQVipInfoRes &roRes, const CUserData &roUserData);

	virtual void OnReplyTimeout() 
	{ 
		OnTimeout(m_oArg, m_oUser); 
	}

	virtual void OnDelayRpc(const CUserData &userData)
	{
		OnDelayReplyRpc( m_oArg, m_oRes, userData);
	}

	virtual CRpc *NewRpc() 
	{
		return CreateRpc();
	}

public:
	QueryQQVipInfoArg m_oArg;
	QueryQQVipInfoRes m_oRes;
};

#endif
