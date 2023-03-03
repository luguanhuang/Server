#ifndef __RPCC2M_SKYCITYENTER_H__
#define __RPCC2M_SKYCITYENTER_H__

// generate by ProtoGen at date: 2016/10/1 10:59:29

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_SKYCITYENTER_TYPE 49485

class RpcC2M_SkyCityEnter : public CRpc
{
private:

	RpcC2M_SkyCityEnter():CRpc(RPCC2M_SKYCITYENTER_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_SkyCityEnter *CreateRpc()
	{
		return new RpcC2M_SkyCityEnter;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const SkyCityEnterArg &roArg, const SkyCityEnterRes &roRes, const CUserData &roUserData);
	void OnTimeout(const SkyCityEnterArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const SkyCityEnterArg &roArg, SkyCityEnterRes &roRes);
	void OnDelayReplyRpc(const SkyCityEnterArg &roArg, SkyCityEnterRes &roRes, const CUserData &roUserData);

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
	SkyCityEnterArg m_oArg;
	SkyCityEnterRes m_oRes;
};

#endif
