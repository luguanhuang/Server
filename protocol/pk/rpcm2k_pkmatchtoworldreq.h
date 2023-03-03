#ifndef __RPCM2K_PKMATCHTOWORLDREQ_H__
#define __RPCM2K_PKMATCHTOWORLDREQ_H__

// generate by ProtoGen at date: 2017/2/2 19:46:07

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2K_PKMATCHTOWORLDREQ_TYPE 65374

class RpcM2K_PkMatchToWorldReq : public CRpc
{
private:

	RpcM2K_PkMatchToWorldReq():CRpc(RPCM2K_PKMATCHTOWORLDREQ_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2K_PkMatchToWorldReq *CreateRpc()
	{
		return new RpcM2K_PkMatchToWorldReq;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const PkMatchToWorldPara &roArg, const PkMatchToWorldRes &roRes, const CUserData &roUserData);
	void OnTimeout(const PkMatchToWorldPara &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const PkMatchToWorldPara &roArg, PkMatchToWorldRes &roRes);
	void OnDelayReplyRpc(const PkMatchToWorldPara &roArg, PkMatchToWorldRes &roRes, const CUserData &roUserData);

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
	PkMatchToWorldPara m_oArg;
	PkMatchToWorldRes m_oRes;
};

#endif
