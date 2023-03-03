#ifndef __RPCC2G_BREAKATLAS_H__
#define __RPCC2G_BREAKATLAS_H__

// generate by ProtoGen at date: 2016/9/1 16:54:12

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2G_BREAKATLAS_TYPE 13728

class RpcC2G_breakAtlas : public CRpc
{
private:

	RpcC2G_breakAtlas():CRpc(RPCC2G_BREAKATLAS_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2G_breakAtlas *CreateRpc()
	{
		return new RpcC2G_breakAtlas;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const breakAtlas &roArg, const breakAtlasRes &roRes, const CUserData &roUserData);
	void OnTimeout(const breakAtlas &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const breakAtlas &roArg, breakAtlasRes &roRes);
	void OnDelayReplyRpc(const breakAtlas &roArg, breakAtlasRes &roRes, const CUserData &roUserData);

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
	breakAtlas m_oArg;
	breakAtlasRes m_oRes;
};

#endif
