#ifndef __RPCM2G_STARTARENAGS_H__
#define __RPCM2G_STARTARENAGS_H__

// generate by ProtoGen at date: 2016/9/11 19:07:13

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCM2G_STARTARENAGS_TYPE 1972

class RpcM2G_StartArenaGs : public CRpc
{
private:

	RpcM2G_StartArenaGs():CRpc(RPCM2G_STARTARENAGS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcM2G_StartArenaGs *CreateRpc()
	{
		return new RpcM2G_StartArenaGs;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const StartArenaGsArg &roArg, const StartArenaGsRes &roRes, const CUserData &roUserData);
	void OnTimeout(const StartArenaGsArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const StartArenaGsArg &roArg, StartArenaGsRes &roRes);
	void OnDelayReplyRpc(const StartArenaGsArg &roArg, StartArenaGsRes &roRes, const CUserData &roUserData);

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
	StartArenaGsArg m_oArg;
	StartArenaGsRes m_oRes;
};

#endif
