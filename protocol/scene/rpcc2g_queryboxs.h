#ifndef __RPCC2G_QUERYBOXS_H__
#define __RPCC2G_QUERYBOXS_H__

// generate by ProtoGen at date: 2017/9/29 11:26:20

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYBOXS_TYPE 12558

class RpcC2G_QueryBoxs : public CRpc
{
private:

	RpcC2G_QueryBoxs():CRpc(RPCC2G_QUERYBOXS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryBoxs *CreateRpc()
	{
		return new RpcC2G_QueryBoxs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryBoxsArg &roArg, const QueryBoxsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryBoxsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryBoxsArg &roArg, QueryBoxsRes &roRes);
	void OnDelayReplyRpc(const QueryBoxsArg &roArg, QueryBoxsRes &roRes, const CUserData &roUserData);

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
	QueryBoxsArg m_oArg;
	QueryBoxsRes m_oRes;
};

#endif
