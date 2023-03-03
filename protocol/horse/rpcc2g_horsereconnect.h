#ifndef __RPCC2G_HORSERECONNECT_H__
#define __RPCC2G_HORSERECONNECT_H__

// generate by ProtoGen at date: 2016/12/14 10:45:40

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_HORSERECONNECT_TYPE 7786

class RpcC2G_HorseReConnect : public CRpc
{
private:

	RpcC2G_HorseReConnect():CRpc(RPCC2G_HORSERECONNECT_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_HorseReConnect *CreateRpc()
	{
		return new RpcC2G_HorseReConnect;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const HorseReConnectArg &roArg, const HorseReConnectRes &roRes, const CUserData &roUserData);
	void OnTimeout(const HorseReConnectArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const HorseReConnectArg &roArg, HorseReConnectRes &roRes);
	void OnDelayReplyRpc(const HorseReConnectArg &roArg, HorseReConnectRes &roRes, const CUserData &roUserData);

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
	HorseReConnectArg m_oArg;
	HorseReConnectRes m_oRes;
};

#endif
