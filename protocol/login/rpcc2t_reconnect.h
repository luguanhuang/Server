#ifndef __RPCC2T_RECONNECT_H__
#define __RPCC2T_RECONNECT_H__

// generate by ProtoGen at date: 2015/6/4 15:41:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2T_RECONNECT_TYPE 28358

class RpcC2T_Reconnect : public CRpc
{
private:

	RpcC2T_Reconnect():CRpc(RPCC2T_RECONNECT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2T_Reconnect *CreateRpc()
	{
		return new RpcC2T_Reconnect;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ReconnArg &roArg, const ReconnRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ReconnArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ReconnArg &roArg, ReconnRes &roRes);
	void OnDelayReplyRpc(const ReconnArg &roArg, ReconnRes &roRes, const CUserData &roUserData);

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
	ReconnArg m_oArg;
	ReconnRes m_oRes;
};

#endif
