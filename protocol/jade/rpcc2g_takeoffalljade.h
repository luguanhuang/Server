#ifndef __RPCC2G_TAKEOFFALLJADE_H__
#define __RPCC2G_TAKEOFFALLJADE_H__

// generate by ProtoGen at date: 2016/7/14 12:06:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_TAKEOFFALLJADE_TYPE 21793

class RpcC2G_TakeOffAllJade : public CRpc
{
private:

	RpcC2G_TakeOffAllJade():CRpc(RPCC2G_TAKEOFFALLJADE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_TakeOffAllJade *CreateRpc()
	{
		return new RpcC2G_TakeOffAllJade;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const TakeOffAllJadeArg &roArg, const TakeOffAllJadeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const TakeOffAllJadeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const TakeOffAllJadeArg &roArg, TakeOffAllJadeRes &roRes);
	void OnDelayReplyRpc(const TakeOffAllJadeArg &roArg, TakeOffAllJadeRes &roRes, const CUserData &roUserData);

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
	TakeOffAllJadeArg m_oArg;
	TakeOffAllJadeRes m_oRes;
};

#endif
