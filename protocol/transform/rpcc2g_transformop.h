#ifndef __RPCC2G_TRANSFORMOP_H__
#define __RPCC2G_TRANSFORMOP_H__

// generate by ProtoGen at date: 2017/9/9 14:10:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TRANSFORMOP_TYPE 7373

class RpcC2G_TransformOp : public CRpc
{
private:

	RpcC2G_TransformOp():CRpc(RPCC2G_TRANSFORMOP_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TransformOp *CreateRpc()
	{
		return new RpcC2G_TransformOp;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TransformOpArg &roArg, const TransformOpRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TransformOpArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TransformOpArg &roArg, TransformOpRes &roRes);
	void OnDelayReplyRpc(const TransformOpArg &roArg, TransformOpRes &roRes, const CUserData &roUserData);

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
	TransformOpArg m_oArg;
	TransformOpRes m_oRes;
};

#endif
