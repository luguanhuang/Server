#ifndef __RPCC2G_FASHIONCOMPOSE_H__
#define __RPCC2G_FASHIONCOMPOSE_H__

// generate by ProtoGen at date: 2016/10/28 22:23:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_FASHIONCOMPOSE_TYPE 46372

class RpcC2G_FashionCompose : public CRpc
{
private:

	RpcC2G_FashionCompose():CRpc(RPCC2G_FASHIONCOMPOSE_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_FashionCompose *CreateRpc()
	{
		return new RpcC2G_FashionCompose;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const FashionComposeArg &roArg, const FashionComposeRes &roRes, const CUserData &roUserData);
	void OnTimeout(const FashionComposeArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const FashionComposeArg &roArg, FashionComposeRes &roRes);
	void OnDelayReplyRpc(const FashionComposeArg &roArg, FashionComposeRes &roRes, const CUserData &roUserData);

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
	FashionComposeArg m_oArg;
	FashionComposeRes m_oRes;
};

#endif
