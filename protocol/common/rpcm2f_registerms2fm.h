#ifndef __RPCM2F_REGISTERMS2FM_H__
#define __RPCM2F_REGISTERMS2FM_H__

// generate by ProtoGen at date: 2017/2/3 21:57:51

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2F_REGISTERMS2FM_TYPE 18662

class RpcM2F_RegisterMs2Fm : public CRpc
{
private:

	RpcM2F_RegisterMs2Fm():CRpc(RPCM2F_REGISTERMS2FM_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2F_RegisterMs2Fm *CreateRpc()
	{
		return new RpcM2F_RegisterMs2Fm;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const RegisterMs2FmArg &roArg, const RegisterMs2FmRes &roRes, const CUserData &roUserData);
	void OnTimeout(const RegisterMs2FmArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const RegisterMs2FmArg &roArg, RegisterMs2FmRes &roRes);
	void OnDelayReplyRpc(const RegisterMs2FmArg &roArg, RegisterMs2FmRes &roRes, const CUserData &roUserData);

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
	RegisterMs2FmArg m_oArg;
	RegisterMs2FmRes m_oRes;
};

#endif
