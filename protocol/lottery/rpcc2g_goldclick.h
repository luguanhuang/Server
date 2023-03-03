#ifndef __RPCC2G_GOLDCLICK_H__
#define __RPCC2G_GOLDCLICK_H__

// generate by ProtoGen at date: 2016/10/31 20:53:00

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_GOLDCLICK_TYPE 12917

class RpcC2G_GoldClick : public CRpc
{
private:

	RpcC2G_GoldClick():CRpc(RPCC2G_GOLDCLICK_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_GoldClick *CreateRpc()
	{
		return new RpcC2G_GoldClick;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GoldClickArg &roArg, const GoldClickRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GoldClickArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GoldClickArg &roArg, GoldClickRes &roRes);
	void OnDelayReplyRpc(const GoldClickArg &roArg, GoldClickRes &roRes, const CUserData &roUserData);

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
	GoldClickArg m_oArg;
	GoldClickRes m_oRes;
};

#endif
