#ifndef __RPCM2N_REGISTERMS2NS_H__
#define __RPCM2N_REGISTERMS2NS_H__

// generate by ProtoGen at date: 2017/6/9 17:30:23

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2N_REGISTERMS2NS_TYPE 31368

class RpcM2N_registerms2ns : public CRpc
{
private:

	RpcM2N_registerms2ns():CRpc(RPCM2N_REGISTERMS2NS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2N_registerms2ns *CreateRpc()
	{
		return new RpcM2N_registerms2ns;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterMs2NsArg &roArg, const RegisterMs2NsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterMs2NsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterMs2NsArg &roArg, RegisterMs2NsRes &roRes);
	void OnDelayReplyRpc(const RegisterMs2NsArg &roArg, RegisterMs2NsRes &roRes, const CUserData &roUserData);

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
	RegisterMs2NsArg m_oArg;
	RegisterMs2NsRes m_oRes;
};

#endif
