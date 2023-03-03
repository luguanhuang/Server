#ifndef __RPCC2G_AUTOBREAKATLAS_H__
#define __RPCC2G_AUTOBREAKATLAS_H__

// generate by ProtoGen at date: 2016/9/1 16:52:57

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_AUTOBREAKATLAS_TYPE 23263

class RpcC2G_AutoBreakAtlas : public CRpc
{
private:

	RpcC2G_AutoBreakAtlas():CRpc(RPCC2G_AUTOBREAKATLAS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_AutoBreakAtlas *CreateRpc()
	{
		return new RpcC2G_AutoBreakAtlas;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const AutoBreakAtlasArg &roArg, const AutoBreakAtlasRes &roRes, const CUserData &roUserData);
	void OnTimeout(const AutoBreakAtlasArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const AutoBreakAtlasArg &roArg, AutoBreakAtlasRes &roRes);
	void OnDelayReplyRpc(const AutoBreakAtlasArg &roArg, AutoBreakAtlasRes &roRes, const CUserData &roUserData);

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
	AutoBreakAtlasArg m_oArg;
	AutoBreakAtlasRes m_oRes;
};

#endif
