#ifndef __RPCC2G_ACTIVATEHAIRCOLOR_H__
#define __RPCC2G_ACTIVATEHAIRCOLOR_H__

// generate by ProtoGen at date: 2017/6/13 20:38:43

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_ACTIVATEHAIRCOLOR_TYPE 52321

class RpcC2G_ActivateHairColor : public CRpc
{
private:

	RpcC2G_ActivateHairColor():CRpc(RPCC2G_ACTIVATEHAIRCOLOR_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_ActivateHairColor *CreateRpc()
	{
		return new RpcC2G_ActivateHairColor;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const ActivateHairColorArg &roArg, const ActivateHairColorRes &roRes, const CUserData &roUserData);
	void OnTimeout(const ActivateHairColorArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const ActivateHairColorArg &roArg, ActivateHairColorRes &roRes);
	void OnDelayReplyRpc(const ActivateHairColorArg &roArg, ActivateHairColorRes &roRes, const CUserData &roUserData);

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
	ActivateHairColorArg m_oArg;
	ActivateHairColorRes m_oRes;
};

#endif
