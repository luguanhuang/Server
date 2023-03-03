#ifndef __RPCC2M_APPLYGUILDARENA_H__
#define __RPCC2M_APPLYGUILDARENA_H__

// generate by ProtoGen at date: 2016/11/3 19:36:08

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCC2M_APPLYGUILDARENA_TYPE 50879

class RpcC2M_applyguildarena : public CRpc
{
private:

	RpcC2M_applyguildarena():CRpc(RPCC2M_APPLYGUILDARENA_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcC2M_applyguildarena *CreateRpc()
	{
		return new RpcC2M_applyguildarena;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const applyguildarenaarg &roArg, const applyguildarenares &roRes, const CUserData &roUserData);
	void OnTimeout(const applyguildarenaarg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const applyguildarenaarg &roArg, applyguildarenares &roRes);
	void OnDelayReplyRpc(const applyguildarenaarg &roArg, applyguildarenares &roRes, const CUserData &roUserData);

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
	applyguildarenaarg m_oArg;
	applyguildarenares m_oRes;
};

#endif
