#ifndef __RPCC2G_QUERYCLIENTIP_H__
#define __RPCC2G_QUERYCLIENTIP_H__

// generate by ProtoGen at date: 2016/12/21 15:34:48

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_QUERYCLIENTIP_TYPE 24918

class RpcC2G_QueryClientIp : public CRpc
{
private:

	RpcC2G_QueryClientIp():CRpc(RPCC2G_QUERYCLIENTIP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_QueryClientIp *CreateRpc()
	{
		return new RpcC2G_QueryClientIp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const QueryClientIpArg &roArg, const QueryClientIpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const QueryClientIpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const QueryClientIpArg &roArg, QueryClientIpRes &roRes);
	void OnDelayReplyRpc(const QueryClientIpArg &roArg, QueryClientIpRes &roRes, const CUserData &roUserData);

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
	QueryClientIpArg m_oArg;
	QueryClientIpRes m_oRes;
};

#endif
