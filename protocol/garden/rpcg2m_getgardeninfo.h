#ifndef __RPCG2M_GETGARDENINFO_H__
#define __RPCG2M_GETGARDENINFO_H__

// generate by ProtoGen at date: 2017/1/7 16:47:41

#include "crpc.h"
#include "pb/project.pb.h"

using namespace KKSG;

#define RPCG2M_GETGARDENINFO_TYPE 40709

class RpcG2M_GetGardenInfo : public CRpc
{
private:

	RpcG2M_GetGardenInfo():CRpc(RPCG2M_GETGARDENINFO_TYPE)
	{
        m_dwTimeout = 5000;
		m_ArgPtr = &m_oArg;
		m_ResPtr = &m_oRes;
	}

public:
	static RpcG2M_GetGardenInfo *CreateRpc()
	{
		return new RpcG2M_GetGardenInfo;
	}

	virtual RpcEndType GetEndType() const;
	virtual void Process(UINT32 dwConnID);

private:
	// implemention as client
	void OnReply(const GetGardenInfoArg &roArg, const GetGardenInfoRes &roRes, const CUserData &roUserData);
	void OnTimeout(const GetGardenInfoArg &roArg, const CUserData &roUserData);

	// implemention as server
	void OnCall(const GetGardenInfoArg &roArg, GetGardenInfoRes &roRes);
	void OnDelayReplyRpc(const GetGardenInfoArg &roArg, GetGardenInfoRes &roRes, const CUserData &roUserData);

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
	GetGardenInfoArg m_oArg;
	GetGardenInfoRes m_oRes;
};

#endif
