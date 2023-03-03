#ifndef __RPCM2G_MSCONSUMEITEMS_H__
#define __RPCM2G_MSCONSUMEITEMS_H__

// generate by ProtoGen at date: 2016/10/4 16:03:56

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_MSCONSUMEITEMS_TYPE 26525

class RpcM2G_MSConsumeItems : public CRpc
{
private:

	RpcM2G_MSConsumeItems():CRpc(RPCM2G_MSCONSUMEITEMS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_MSConsumeItems *CreateRpc()
	{
		return new RpcM2G_MSConsumeItems;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ConsumeItemsArg &roArg, const ConsumeItemsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ConsumeItemsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ConsumeItemsArg &roArg, ConsumeItemsRes &roRes);
	void OnDelayReplyRpc(const ConsumeItemsArg &roArg, ConsumeItemsRes &roRes, const CUserData &roUserData);

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
	ConsumeItemsArg m_oArg;
	ConsumeItemsRes m_oRes;
};

#endif
