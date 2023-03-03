#ifndef __RPCC2G_COMMENDFIRSTPASS_H__
#define __RPCC2G_COMMENDFIRSTPASS_H__

// generate by ProtoGen at date: 2016/8/24 17:24:53

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_COMMENDFIRSTPASS_TYPE 8467

class RpcC2G_CommendFirstPass : public CRpc
{
private:

	RpcC2G_CommendFirstPass():CRpc(RPCC2G_COMMENDFIRSTPASS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_CommendFirstPass *CreateRpc()
	{
		return new RpcC2G_CommendFirstPass;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const CommendFirstPassArg &roArg, const CommendFirstPassRes &roRes, const CUserData &roUserData);
	void OnTimeout(const CommendFirstPassArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const CommendFirstPassArg &roArg, CommendFirstPassRes &roRes);
	void OnDelayReplyRpc(const CommendFirstPassArg &roArg, CommendFirstPassRes &roRes, const CUserData &roUserData);

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
	CommendFirstPassArg m_oArg;
	CommendFirstPassRes m_oRes;
};

#endif
